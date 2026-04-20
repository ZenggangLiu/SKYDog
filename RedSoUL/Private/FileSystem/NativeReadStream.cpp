/// Self header
#include "FileSystem/NativeReadStream.hpp"


NativeReadStream::NativeReadStream (
    const char * const absolute_file_name)
{
    open(absolute_file_name, AccessMode::READ_ONLY_ACCESS_MODE);
}


NativeReadStream::~NativeReadStream ()
{

}


NativeReadStream &
NativeReadStream::operator >> (
    char & data)
{
    return (*this) >> (uint8_t&)data;
}


NativeReadStream &
NativeReadStream::operator >> (
    int8_t & data)
{
    return (*this) >> (uint8_t&)data;
}


NativeReadStream &
NativeReadStream::operator >> (
    int16_t & data)
{
    return (*this) >> (uint16_t&)data;
}


NativeReadStream &
NativeReadStream::operator >> (
    int32_t & data)
{
    return (*this) >> (uint32_t&)data;
}


NativeReadStream &
NativeReadStream::operator >> (
    int64_t & data)
{
    return (*this) >> (uint64_t&)data;
}


NativeReadStream &
NativeReadStream::operator >> (
    uint8_t & data)
{
    read(&data, sizeof(uint8_t), 0, sizeof(uint8_t));
    return (*this);
}


NativeReadStream &
NativeReadStream::operator >> (
    uint16_t & data)
{
    read((uint8_t*)&data, sizeof(uint16_t), 0, sizeof(uint16_t));
    return (*this);
}


NativeReadStream &
NativeReadStream::operator >> (
    uint32_t & data)
{
    read((uint8_t*)&data, sizeof(uint32_t), 0, sizeof(uint32_t));
    return (*this);
}


NativeReadStream &
NativeReadStream::operator >> (
    uint64_t & data)
{
    read((uint8_t*)&data, sizeof(uint64_t), 0, sizeof(uint64_t));
    return (*this);
}


NativeReadStream &
NativeReadStream::operator >> (
    float & data)
{
    read((uint8_t*)&data, sizeof(float), 0, sizeof(float));
    return (*this);
}


NativeReadStream &
NativeReadStream::operator >> (
    double & data)
{
    read((uint8_t*)&data, sizeof(double), 0, sizeof(double));
    return (*this);
}
