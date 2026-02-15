/***************************************************************************************
                                                                                        
        *          .               *                              .               *     
        ███████╗██╗  ██╗██╗   ██╗        ██████╗  ██████╗  ██████╗         *            
        ██╔════╝██║ ██╔╝╚██╗ ██╔╝        ██╔══██╗██╔═══██╗██╔════╝                      
        ███████╗█████╔╝  ╚████╔╝         ██║  ██║██║   ██║██║  ███╗        .            
        ╚════██║██╔═██╗   ╚██╔╝          ██║  ██║██║   ██║██║   ██║                     
        ███████║██║  ██╗    ██║           ██████╔╝╚██████╔╝╚██████╔╝         *          
        ╚══════╝╚═╝  ╚═╝    ╚═╝           ╚═════╝  ╚═════╝  ╚═════╝                     
                                                                                        
        <~~~               .        SKY Dog Game                      ~~~>        *     
                                Real-Time | Cross-Platform           .                  
----------------------------------------------------------------------------------------
                                                                                        
                                  ,,                                                    
                  __           o-°°|\_____/)                                            
    Author:   (___()'`; Zee...  \_/|_)     )                                            
              /,    /`             \  __  /                                             
              \\"--\\              (_/ (_/                                              
    Created:  11/02/26  @  2:59 PM
    FileName: X2TeaEncoder.hpp @ RedSoUL Project
    History:
             - created by: 11/02/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


#include "Common/CommonDefines.hpp"  /// ARRAY_SIZE
#include "DataType/BuiltInTypes.hpp" /// UByte, UInt


struct XXTEA
{
    /// 编码给定数据中的内容
    ///
    /// @param[in] key
    ///      编码时使用的Key
    /// @param[in] key_length
    ///      Key的长度(字节数)
    ///      NOTE：最长的Key为16字节(128位)
    /// @param[in] data_ptr
    ///      原始数据的指针
    /// @param[in] data_size
    ///      原始数据的长度(以UInt来衡量)
    ///      NOTE：最短的数据长度为2
    ///
    static
    void
    encode (
        const UByte * const key,
        const UInt          key_length,
        UInt * const        data_ptr,
        const UInt          data_size);

    /// 解码给定数据中的内容
    ///
    /// @param[in] key
    ///      解码时使用的Key
    /// @param[in] key_length
    ///      Key的长度(字节数)
    ///      NOTE：最长的Key为16字节(128位)
    /// @param[in] data_ptr
    ///      编码数据的指针
    /// @param[in] data_size
    ///      编码数据的长度(以UInt来衡量)
    ///      NOTE：最短的数据长度为2
    ///
    static
    void
    decode (
        const UByte * const key,
        const UInt          key_length,
        UInt * const        data_ptr,
        const UInt          data_size);
};


#define XXTEA_ENCODE(key, key_length, data_ptr) \
    XXTEA::encode(key, key_length, data_ptr, ARRAY_SIZE(data_ptr));
#define XXTEA_DECODE(key, key_length, data_ptr) \
    XXTEA::decode(key, key_length, data_ptr, ARRAY_SIZE(data_ptr));
