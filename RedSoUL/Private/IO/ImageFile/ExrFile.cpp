/// System headers
#include <cstring>  /// std::strlen
#include <vector>
/// Library headers
#include "Assert/RuntimeAssert.hpp"
#include "Common/CommonDefines.hpp"   /// FOUR_CC_32, INLINE_FUNCTION
#include "DataType/Half.hpp"
#include "DataType/HdrColor.hpp"
#include "FileSystem/FileName.hpp"
#include "FileSystem/NativeWriteStream.hpp"
/// Self header
#include "IO/ImageFile/ExrFile.hpp"


/// Pixel中每个色彩通道数据类型
typedef half ChannelDataTypeT;
/// Scanline数据Offset类型
typedef uint64_t OffsetDataTypeT;



// MARK:== CONSTANTS定义 == //
/// Exr文件的Magic number: { 0x76, 0x2f, 0x31, 0x01 }
static constexpr uint32_t MAGIC_NUMBER = FOUR_CC_32(0x76, 0x2F, 0x31, 0x01);
/// Version Field数位排布(Bits Layout):
///
/// 0               7  8          9                          10
/// +---------------+-----+----------------+-------------------------------------+
/// | EXR Version   |  0  |    Use Tile    |            Use Long Name            |
/// +---------------+-----+----------------+-------------------------------------+
/// Version: 2            0: Use Scanline  0: Attribute/Channel名字最长31字节
///                       1: Use Tite      1: Attribute/Channel名字最长255字节
///
///           11                    12            13          31
/// +--------------------+------------------------+-----------+
/// |     Deep Image     |        Mulit-Part      |  Reserved |
/// +--------------------+------------------------+-----------+
/// 0: RGB               0: 只有一个数据Part
/// 1: Deep Image        1: 多个数据Part
///
/// 由于我们只支持:
/// - 单一数据Part, 基于ScanLine的RGB数据而且不使用Long Name
/// ==> 因此:
/// - Version Field为: { 0x02, 0x00, 0x00, 0x00 }
static constexpr uint32_t VERSION_FIELD = FOUR_CC_32(0x02, 0x00, 0x00, 0x00);
static constexpr uint32_t CHANNEL_COUNT = 3;
/// Pixel每个色彩通道的字节大小
static constexpr size_t   CHANNEL_BYTE_SIZE = sizeof(ChannelDataTypeT);
/// ‘\0’ 字符
static constexpr uint8_t  ENDING_ZERO = 0;

static constexpr char RED_CHANNEL_NAME[]            = "R";
static constexpr char GREEN_CHANNEL_NAME[]          = "G";
static constexpr char BLUE_CHANNEL_NAME[]           = "B";
static constexpr char FLOAT_DATA_TYPE[]             = "float";
static constexpr char BOX2I_TYPE[]                  = "box2i";
static constexpr char V2F_TYPE[]                    = "v2f";
static constexpr char CHANNEL_LIST_NAME[]           = "channels";
static constexpr char CHANNELS_TYPE[]               = "chlist";
static constexpr char COMPRESSION_NAME[]            = "compression";
static constexpr auto COMPRESSION_TYPE              = COMPRESSION_NAME;
static constexpr char DATA_WINDOW_NAME[]            = "dataWindow";
static constexpr auto DATA_WINDOW_TYPE              = BOX2I_TYPE;
static constexpr char DISPLAY_WINDOW_NAME[]         = "displayWindow";
static constexpr auto DISPLAY_WINDOW_TYPE           = DATA_WINDOW_TYPE;
static constexpr char PIXEL_ASPECT_RATIO_NAME[]     = "pixelAspectRatio";
static constexpr auto PIXEL_ASPECT_RATIO_TYPE       = FLOAT_DATA_TYPE;
static constexpr char SCAN_LINE_ORDER_NAME[]        = "lineOrder";
static constexpr auto SCAN_LINE_ORDER_TYPE          = SCAN_LINE_ORDER_NAME;
static constexpr char SCREEN_WINDOW_CENTER_NAME[]   = "screenWindowCenter";
static constexpr auto SCREEN_WINDOW_CENTER_TYPE     = V2F_TYPE;
static constexpr char SCREEN_WINDOW_WIDTH_NAME[]    = "screenWindowWidth";
static constexpr auto SCREEN_WINDOW_WIDTH_TYPE      = FLOAT_DATA_TYPE;



// MARK:== ExrFileStream定义 == //
struct ExrFileStream
{
    /// 色彩数据通道类型
    enum class DataChannelMode : uint8_t
    {
        SINGLE_CHANNEL,
        RGB_CHANNELS,
    };

    /// 色彩数据存储的类型(Exr要求为32位)
    enum class DataStorageMode : uint32_t
    {
        UINT32 = 0,
        HALF   = 1,
        FLOAT  = 2,
    };

    /// 色彩数据视觉的类型(Exr要求为8位)
    enum class DataPerceptualMode : uint8_t
    {
        NON_LINEAR = 0,
        LINEAR     = 1,
    };

    /// 色彩数据压缩类型(Exr要求为8位)
    enum class DataCompressMode : uint8_t
    {
        NO_COMPRESSION = 0,
    };

    /// Scanline数据模式(Exr要求为8位)
    enum class ScaneLineMode : uint8_t
    {
        /// 由于Exr使用如下坐标系:
        ///
        /// TOP------------> x
        ///  |
        ///  |
        ///  |
        ///  |
        ///  v
        /// BOTTOM/y
        ///
        /// 数据从TOP-->BOTTOM方向存储: Y的数值增加
        TOP_DOWN = 0,

        /// 数据从BOTTOM-->TOP方向存储: Y的数值减少
        /// NOTE:
        /// * GIMP不支持此模式
        /// * MacOS支持此模式，但是忽略ScanLineBlock中的Y Coordinate的数值
        /// * 我们只使用TOP_DOWN模式，而且使Y Coordinate递增
        BOTTOM_UP = 1,
    };

    struct Box2i
    {
        int32_t x_min = 0;
        int32_t y_min = 0;
        int32_t x_max = 0;
        int32_t y_max = 0;
    };

    struct v2f
    {
        float x = 0.0f;
        float y = 0.0f;
    };

    struct AttributeData
    {
        INLINE_FUNCTION
        virtual
        void
        writeData (NativeWriteStream&) const = 0;
    };

    /// Attribute列表中的一个成员
    struct Attribute
    {
        /// 以\0结尾的名称
        const char *          attr_name;
        /// 以\0结尾的类型
        const char *          attr_type;
        /// Channel列表数据的大小(字节数)
        uint32_t              data_size;
        /// Channel列表
        const AttributeData * data_ptr;

        INLINE_FUNCTION
        uint32_t
        size () const
        {
            /// 结尾的\0需要计算在内
            const uint32_t name_byte_size =
                (uint32_t)std::strlen((const char*)attr_name) + 1;
            const uint32_t type_byte_size =
                (uint32_t)std::strlen((const char*)attr_type) + 1;

            return name_byte_size + type_byte_size +
                   (uint32_t)sizeof(data_size) + data_size;
        }


        INLINE_FUNCTION
        void
        writeAttribute (
            NativeWriteStream & file_stream)
        {
            // 输出属性名称
            file_stream << attr_name << ENDING_ZERO;
            // 输出属性类型名称
            file_stream << attr_type << ENDING_ZERO;
            // 输出属性数据大小
            file_stream << data_size;
            // 输出属性数据
            data_ptr->writeData(file_stream);
        }
    };

    /// RGB通道信息
    struct ChannelInfo
    {
        /// \0结尾的名称
        const char *       channel_name;
        /// 当前Channel数据的类型
        DataStorageMode    storage_mode      = DataStorageMode::HALF;
        /// 当前数据是否在Perceptual上线性: 0表示非，1表示是
        DataPerceptualMode perceptual_linear = DataPerceptualMode::NON_LINEAR;
        const uint8_t      reserved[3]       = { 0, 0, 0 };
        /// 每几个Pixel具有一个数据
        uint32_t           x_sample_rate     = 1;
        uint32_t           y_sample_rate     = 1;

        INLINE_FUNCTION
        uint32_t
        size () const
        {
            return (uint32_t)(std::strlen((const char*)channel_name) + 1       +
                              sizeof(storage_mode) + sizeof(perceptual_linear) +
                              sizeof(reserved) + sizeof(x_sample_rate)         +
                              sizeof(y_sample_rate));
        }

        INLINE_FUNCTION
        void
        writeChannelInfo (
            NativeWriteStream & file_stream) const
        {
            file_stream << channel_name << ENDING_ZERO;
            file_stream.write(
                (const uint8_t*)&storage_mode, sizeof(storage_mode),
                0, sizeof(storage_mode));
            file_stream.write(
                (const uint8_t*)&perceptual_linear, sizeof(perceptual_linear),
                0, sizeof(perceptual_linear));
            file_stream.write(reserved, sizeof(reserved), 0, sizeof(reserved));
            file_stream << x_sample_rate;
            file_stream << y_sample_rate;
        }
    };

    /// Rgb scan line data
    struct RgbScanline
    {
        /// 此scanline数据在EXR坐标系内的Y的坐标
        uint32_t        y_coord;
        /// 此scanline中所有Pixel单一通道数据的大小(字节数)
        uint32_t        channel_data_size;
        /// Pixel数据
        /// NOTE:
        /// - Channel数据顺序: B G R
        const uint8_t * blue_data_ptr;
        const uint8_t * green_data_ptr;
        const uint8_t * red_data_ptr;

        /// @param[in] pixel_data_byte_size
        ///     所有Pixel数据的字节大小
        INLINE_FUNCTION
        static
        uint32_t
        size_with_data (
            const uint32_t pixel_data_byte_size)
        {
            return sizeof(y_coord) + sizeof(channel_data_size) +
                   pixel_data_byte_size;
        }


        INLINE_FUNCTION
        void
        writeScanlineBlock (
            NativeWriteStream & file_stream) const
        {
            const uint32_t pixel_data_byte_size = channel_data_size * CHANNEL_COUNT;
            file_stream << y_coord;
            file_stream << pixel_data_byte_size;
            file_stream.write(blue_data_ptr,  channel_data_size, 0, channel_data_size);
            file_stream.write(green_data_ptr, channel_data_size, 0, channel_data_size);
            file_stream.write(red_data_ptr,   channel_data_size, 0, channel_data_size);
        }
    };

    // === 各种Attribute Data === //
    struct ChannelList : public AttributeData
    {
        /// Channel信息列表
        ChannelInfo * channel_info_list;
        /// Channel信息的个数
        uint8_t       channel_info_count;

        INLINE_FUNCTION
        uint32_t
        size () const
        {
            uint32_t exp_byte_size = 0;
            for (uint8_t c = 0; c < channel_info_count; ++c)
            {
                const ChannelInfo & channel_info = channel_info_list[c];
                exp_byte_size += channel_info.size();
            }

            exp_byte_size += 1; /// 信息列表的结尾: \0 字符

            return exp_byte_size;
        }

        INLINE_FUNCTION
        virtual
        void
        writeData (
            NativeWriteStream & file_stream) const final
        {
            /// 输出所以Channel Info
            for (uint8_t c = 0; c < channel_info_count; ++c)
            {
                const ChannelInfo & channel_info = channel_info_list[c];
                channel_info.writeChannelInfo(file_stream);
            }

            /// 输出信息列表的结尾: \0字符
            file_stream << ENDING_ZERO;
        }
    };

    struct CompressionInfo : public AttributeData
    {
        DataCompressMode mode = DataCompressMode::NO_COMPRESSION;

        INLINE_FUNCTION
        uint32_t
        size () const
        {
            return sizeof(mode);
        }

        INLINE_FUNCTION
        virtual
        void
        writeData (
            NativeWriteStream & file_stream) const final
        {
            file_stream.write((const uint8_t*)&mode, sizeof(mode), 0, sizeof(mode));
        }
    };

    struct WindowInfo : public AttributeData
    {
        Box2i dimension;

        INLINE_FUNCTION
        uint32_t
        size () const
        {
            return sizeof(dimension);
        }

        INLINE_FUNCTION
        virtual
        void
        writeData (
            NativeWriteStream & file_stream) const final
        {
            file_stream << dimension.x_min;
            file_stream << dimension.y_min;
            file_stream << dimension.x_max;
            file_stream << dimension.y_max;
        }
    };

    /// 定义每个Scan Line如何存储
    struct ScanLineOrder : public AttributeData
    {
        ScaneLineMode mode = ScaneLineMode::TOP_DOWN;

        INLINE_FUNCTION
        uint32_t
        size () const
        {
            return sizeof(mode);
        }

        INLINE_FUNCTION
        virtual
        void
        writeData (
            NativeWriteStream & file_stream) const final
        {
            file_stream.write((const uint8_t*)&mode, sizeof(mode), 0, sizeof(mode));
        }
    };

    /// 定义每个Pixel的宽高比
    /// - 通常为1: 我们使用正方形Pixel
    struct PixelAspectRatio : public AttributeData
    {
        float aspect_ratio = 1.0f;

        INLINE_FUNCTION
        uint32_t
        size () const
        {
            return sizeof(aspect_ratio);
        }

        INLINE_FUNCTION
        virtual
        void
        writeData (
            NativeWriteStream & file_stream) const final
        {
            file_stream << aspect_ratio;
        }
    };

    /// Z=1投影面的中心
    struct ScreenWindowCenter : public AttributeData
    {
        v2f center;

        INLINE_FUNCTION
        uint32_t
        size () const
        {
            return sizeof(center);
        }

        INLINE_FUNCTION
        virtual
        void
        writeData (
            NativeWriteStream & file_stream) const final
        {
            file_stream << center.x;
            file_stream << center.y;
        }
    };

    // Z=1投影面的宽度
    struct ScreenWindowWidth : public AttributeData
    {
        float width = 1.0f;

        INLINE_FUNCTION
        uint32_t
        size () const
        {
            return sizeof(width);
        }

        INLINE_FUNCTION
        virtual
        void
        writeData (
            NativeWriteStream & file_stream) const final
        {
            file_stream << width;
        }
    };

    NativeWriteStream & exr_stream;
    /// 生成的Exr中记录的偏移
    OffsetDataTypeT     exr_offset;

    ExrFileStream (
        NativeWriteStream & file_stream)
    :
        exr_stream(file_stream),
        exr_offset(0)
    {

    }

    void
    generateHeader ()
    {
        /// 输出Magic Number
        exr_stream << MAGIC_NUMBER;
        /// 输出Version Field
        exr_stream << VERSION_FIELD;

        exr_offset += sizeof(MAGIC_NUMBER) + sizeof(VERSION_FIELD);
    }

    void
    generateAttributes (
        const uint32_t        image_width,
        const uint32_t        image_height,
        const DataChannelMode channel_mode)
    {
        Attribute new_attribute;

        // === 输出channel list属性 === //
        /// +-----------------------------------------------------------------------------------+
        /// |                                   Channel LIST                                    |
        /// +--------------+--------------+------------+----------+--------------+--------------+
        /// | Channel Name | Storage Mode | Perceptual | Reserved | XSample Rate | YSample rate |
        /// +--------------+--------------+------------+----------+--------------+--------------+
        /// |                                       ...                                         |
        /// +--------------+--------------+------------+----------+--------------+--------------+
        /// | Channel Name | Storage Mode | Perceptual | Reserved | XSample Rate | YSample rate |
        /// +--------------+--------------+------------+----------+--------------+--------------+
        /// |                                       \0                                          |
        /// +-----------------------------------------------------------------------------------+
        ///
        ChannelInfo channel_info_lists[3];
        ChannelList channel_list;
        channel_list.channel_info_list = &channel_info_lists[0];

        switch (channel_mode)
        {
            case DataChannelMode::SINGLE_CHANNEL:
            {
                /// NOTE: 对于单一通道数据, 使用RED通道存储
                channel_info_lists[0].channel_name = RED_CHANNEL_NAME;
                channel_list.channel_info_count = 1;
                break;
            }

            case DataChannelMode::RGB_CHANNELS:
            {
                /// 按照通道Channel的名称排序: BGR 顺序存储RGB数据
                channel_info_lists[0].channel_name = BLUE_CHANNEL_NAME;
                channel_info_lists[1].channel_name = GREEN_CHANNEL_NAME;
                channel_info_lists[2].channel_name = RED_CHANNEL_NAME;
                channel_list.channel_info_count = 3;
                break;
            }

            default:
            {
                RUNTIME_ASSERT(false, "Unknown channel mode");
                break;
            }
        }

        new_attribute.attr_name = CHANNEL_LIST_NAME;
        new_attribute.attr_type = CHANNELS_TYPE;
        new_attribute.data_size = channel_list.size();
        new_attribute.data_ptr  = &channel_list;
        new_attribute.writeAttribute(exr_stream);
        exr_offset += new_attribute.size();


        // === 输出如下Attribute List属性 === //
        /// +-----------------------------------------------------------------------+
        /// |                             Attribute LIST                            |
        /// +----------------+----------------+--------------------+----------------+
        /// | Attribute Name | Attribute Type | Data Size In Bytes | Attribute Data |
        /// +----------------+----------------+--------------------+----------------+
        /// |   String  | \0 |   String  | \0 |      uint32_t      |      ~~~       |
        /// +----------------+----------------+--------------------+----------------+
        /// |                                  ...                                  |
        /// +----------------+----------------+--------------------+----------------+
        /// | Attribute Name | Attribute Type | Data Size In Bytes | Attribute Data |
        /// +----------------+----------------+--------------------+----------------+
        /// |                                 \0                                    |
        /// +-----------------------------------------------------------------------+
        ///
        // === 输出compression属性 === //
        CompressionInfo compression_info;

        new_attribute.attr_name = COMPRESSION_NAME;
        new_attribute.attr_type = COMPRESSION_TYPE;
        new_attribute.data_size = compression_info.size();
        new_attribute.data_ptr  = &compression_info;
        new_attribute.writeAttribute(exr_stream);
        exr_offset += new_attribute.size();

        // === 输出dataWindow属性 === //
        WindowInfo data_window_info;
        data_window_info.dimension.x_max = image_width  - 1;
        data_window_info.dimension.y_max = image_height - 1;

        new_attribute.attr_name = DATA_WINDOW_NAME;
        new_attribute.attr_type = DATA_WINDOW_TYPE;
        new_attribute.data_size = data_window_info.size();
        new_attribute.data_ptr  = &data_window_info;
        new_attribute.writeAttribute(exr_stream);
        exr_offset += new_attribute.size();

        // === 输出displayWindow属性 === //
        WindowInfo display_window_info;
        display_window_info.dimension.x_max = image_width  - 1;
        display_window_info.dimension.y_max = image_height - 1;

        new_attribute.attr_name = DISPLAY_WINDOW_NAME;
        new_attribute.attr_type = DISPLAY_WINDOW_TYPE;
        new_attribute.data_size = display_window_info.size();
        new_attribute.data_ptr  = &display_window_info;
        new_attribute.writeAttribute(exr_stream);
        exr_offset += new_attribute.size();

        // === 输出lineOrder属性 === //
        ScanLineOrder scanline_order_info;

        new_attribute.attr_name = SCAN_LINE_ORDER_NAME;
        new_attribute.attr_type = SCAN_LINE_ORDER_TYPE;
        new_attribute.data_size = scanline_order_info.size();
        new_attribute.data_ptr  = &scanline_order_info;
        new_attribute.writeAttribute(exr_stream);
        exr_offset += new_attribute.size();

        // === 输出pixelAspectRatio属性 === //
        PixelAspectRatio pixel_aspect_ratio_info;

        new_attribute.attr_name = PIXEL_ASPECT_RATIO_NAME;
        new_attribute.attr_type = PIXEL_ASPECT_RATIO_TYPE;
        new_attribute.data_size = pixel_aspect_ratio_info.size();
        new_attribute.data_ptr  = &pixel_aspect_ratio_info;
        new_attribute.writeAttribute(exr_stream);
        exr_offset += new_attribute.size();

        // === 输出screenWindowCenter属性 === ///
        ScreenWindowCenter screen_center_info;

        new_attribute.attr_name = SCREEN_WINDOW_CENTER_NAME;
        new_attribute.attr_type = SCREEN_WINDOW_CENTER_TYPE;
        new_attribute.data_size = screen_center_info.size();
        new_attribute.data_ptr  = &screen_center_info;
        new_attribute.writeAttribute(exr_stream);
        exr_offset += new_attribute.size();

        // === 输出screenWindowWidth属性 === //
        ScreenWindowWidth screen_width_info;

        new_attribute.attr_name = SCREEN_WINDOW_WIDTH_NAME;
        new_attribute.attr_type = SCREEN_WINDOW_WIDTH_TYPE;
        new_attribute.data_size = screen_width_info.size();
        new_attribute.data_ptr  = &screen_width_info;
        new_attribute.writeAttribute(exr_stream);
        exr_offset += new_attribute.size();

        /// 输出\0标记Header结束
        exr_stream << ENDING_ZERO;
        ++exr_offset;
    }

    /// 生成图片中的每一行(每个Scanline)数据的偏移表
    void
    generateScanlineDataOffsetTable (
        const uint32_t image_width,
        const uint32_t image_height)
    {
        /// +-------------------------------+
        /// | Scan Line Data Offset Table   | ----+
        /// +---------------------+---------+     |
        /// | Scan Line Data List |               |
        /// |    (Image Data)     |               |
        /// +---------------------+               |
        /// | First Row Data      | <-------------+
        /// +---------------------+
        /// | Second Row Data     |
        /// +---------------------+
        /// |         ~~~         |
        /// +---------------------+
        /// | nth Row Data        |
        /// +---------------------+
        ///
        /// 一行中所有Pixel数据的字节大小(SINGLE Channel)
        const uint32_t channel_data_byte_size = (uint32_t)CHANNEL_BYTE_SIZE * image_width;
        /// 一行中所有Pixel数据的字节大小(RGB Channels)
        const uint32_t pixel_data_byte_size = channel_data_byte_size * CHANNEL_COUNT;
        /// RgbScanline实例的字节大小
        const uint32_t rgbscanline_byte_size = RgbScanline::size_with_data(pixel_data_byte_size);

        /// 'Scan Line Data Offset Table'的字节大小: 每一个Offset使用 OffsetDataTypeT 保存
        /// +-------------------------------+
        /// | Scan Line Data Offset Table   |
        /// +-------------------------------+
        const uint32_t offset_table_byte_size = sizeof(OffsetDataTypeT) * image_height;

        /// 第一个scanline数据的偏移
        exr_offset += offset_table_byte_size;

        for (uint32_t row_idx = 0; row_idx < image_height; ++row_idx)
        {
            exr_stream << exr_offset;
            exr_offset += rgbscanline_byte_size;
        }
    }

    void
    generateScanlineData (
        const uint32_t         image_width,
        const uint32_t         image_height,
        const HdrColor * const image_data)
    {
        /// 将数据转换到如下坐标系, 并且将各个Channel的数据分开: 蓝色通道, 绿色通道, 红色通道
        ///
        /// TOP------------> x
        ///  |
        ///  |
        ///  |
        ///  |
        ///  v
        /// BOTTOM/y
        ///
        std::vector<half> flipped_red;
        std::vector<half> flipped_green;
        std::vector<half> flipped_blue;
        flipped_red.reserve  (image_width * image_height);
        flipped_green.reserve(image_width * image_height);
        flipped_blue.reserve (image_width * image_height);

        for (int32_t row_idx = image_height - 1; row_idx >= 0; --row_idx)
        {
            for (uint32_t col_idx = 0; col_idx < image_width; ++col_idx)
            {
                const uint32_t data_idx   = row_idx * image_width + col_idx;
                const HdrColor color_data = image_data[data_idx];
                flipped_red.push_back  (color_data.r);
                flipped_green.push_back(color_data.g);
                flipped_blue.push_back (color_data.b);
            }
        }

        /// 一行中所有Pixel数据的字节大小(SINGLE Channel)
        const uint32_t channel_data_byte_size = (uint32_t)CHANNEL_BYTE_SIZE * image_width;

        /// 逐行输出scanline数据(按Y递增输出:即从TOP --> BOTTOM)
        RgbScanline scanline_data;
        for (uint32_t row_idx = 0; row_idx < image_height; ++row_idx)
        {
            const uint32_t data_idx = row_idx * image_width;
            scanline_data.y_coord = row_idx;
            scanline_data.channel_data_size = channel_data_byte_size;
            scanline_data.red_data_ptr = (const uint8_t*)(&flipped_red[data_idx]);
            scanline_data.green_data_ptr = (const uint8_t*)(&flipped_green[data_idx]);
            scanline_data.blue_data_ptr = (const uint8_t*)(&flipped_blue[data_idx]);
            scanline_data.writeScanlineBlock(exr_stream);
        }
    }
};



// MARK:== ExrFile定义 == //
bool
ExrFile::write_to (
    const char * const     abs_file_name,
    const uint32_t         image_width,
    const uint32_t         image_height,
    const HdrColor * const pixel_array,
    const uint32_t         pixel_count)
{
    RUNTIME_ASSERT( abs_file_name, "File name can not be NULL!!");
    RUNTIME_ASSERT(*abs_file_name, "File name can not be empty!!");
    RUNTIME_ASSERT(pixel_array, "No pixel data!!");
    RUNTIME_ASSERT(pixel_count >= (image_width * image_height),
                   "No enough pixel data!! Expected #pixel: %u, but given #pixel: %u",
                   image_width * image_height, pixel_count);

    char buffer[1024];
    /// 确保使用'.exr'文件扩展符
    FileName::append_file_extension(abs_file_name, "exr", buffer, sizeof(buffer));

    /// 创建WriteOnly文件流
    NativeWriteStream exr_file(buffer);
    if (exr_file.is_opened())
    {
        ExrFileStream exr_stream(exr_file);
        exr_stream.generateHeader();
        exr_stream.generateAttributes(
            image_width, image_height,
            ExrFileStream::DataChannelMode::RGB_CHANNELS);
        exr_stream.generateScanlineDataOffsetTable(image_width, image_height);
        exr_stream.generateScanlineData(image_width, image_height, pixel_array);
        /// 关闭文件流
        exr_file.close();
        return true;
    }
    else
    {
        return false;
    }
}
