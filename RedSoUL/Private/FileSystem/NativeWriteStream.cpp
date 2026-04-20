/// System headers
#include <cstring>  /// std::strlen
/// Library headers
#include "Assert/RuntimeAssert.hpp"
/// Self header
#include "FileSystem/NativeWriteStream.hpp"


NativeWriteStream::NativeWriteStream (
    const char * const absolute_file_name)
{
    open(absolute_file_name, AccessMode::WRITE_ONLY_ACCESS_MODE);
}


NativeWriteStream::~NativeWriteStream ()
{

}


NativeWriteStream &
NativeWriteStream::operator << (
    const char data)
{
    return (*this) << (uint8_t)data;
}


NativeWriteStream &
NativeWriteStream::operator << (
    const int8_t data)
{
    return (*this) << (uint8_t)data;
}


NativeWriteStream &
NativeWriteStream::operator << (
    const int16_t data)
{
    return (*this) << (uint16_t)data;
}


NativeWriteStream &
NativeWriteStream::operator << (
    const int32_t data)
{
    return (*this) << (uint32_t)data;
}


NativeWriteStream &
NativeWriteStream::operator << (
    const int64_t data)
{
    return (*this) << (uint64_t)data;
}


NativeWriteStream &
NativeWriteStream::operator << (
    const uint8_t data)
{
    write(&data, sizeof(uint8_t), 0, sizeof(uint8_t));
    return (*this);
}


NativeWriteStream &
NativeWriteStream::operator << (
    const uint16_t data)
{
    write((const uint8_t*)&data, sizeof(uint16_t), 0, sizeof(uint16_t));
    return (*this);
}


NativeWriteStream &
NativeWriteStream::operator << (
    const uint32_t data)
{
    write((const uint8_t*)&data, sizeof(uint32_t), 0, sizeof(uint32_t));
    return (*this);
}


NativeWriteStream &
NativeWriteStream::operator << (
    const uint64_t data)
{
    write((const uint8_t*)&data, sizeof(uint64_t), 0, sizeof(uint64_t));
    return (*this);
}


NativeWriteStream &
NativeWriteStream::operator << (
    const float data)
{
    write((const uint8_t*)&data, sizeof(float), 0, sizeof(float));
    return (*this);
}


NativeWriteStream &
NativeWriteStream::operator << (
    const double data)
{
    write((const uint8_t*)&data, sizeof(double), 0, sizeof(double));
    return (*this);
}


NativeWriteStream &
NativeWriteStream::operator << (
    const char * const data)
{
    const uint32_t data_length = (uint32_t)std::strlen(data);
    write((const uint8_t*)data, data_length, 0, data_length);
    return (*this);
}
