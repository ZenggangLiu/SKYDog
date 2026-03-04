#include <cstring>
#include "assert/runtime-assert.h"
#include "common/common-defines.h"
#include "hash/runtime-hash.h"
#include "hash/static-hash.h"
#include "mathe/mathe-helper.h"
#include "memory/memory-helper.h"
#include "text/static-string-depot.h"


static StaticStringIdT EMPTY_STRING_ID = StaticStringIdT{ STATIC_HASH(STATIC_STRING_SEED, "") };


StaticStringDepot & StaticStringDepot::ref()
{
    static StaticStringDepot s_instance;
    return s_instance;
}


void StaticStringDepot::stats() const
{
#if (PROFILING_MODE == 1)
    std::printf("<<static string depot>>: used %.2fmb memory\n", std::get<0>(mAllocator.memoryStats()) / 1024);
#endif
}


StaticStringIdT StaticStringDepot::getEmptyStringId() const
{
    return EMPTY_STRING_ID;
}


uint16_t StaticStringDepot::length(
    const StringKeyTypeT key) const
{
    RUNTIME_ASSERT(key != INVALID_STRING_KEY, "Invalid string key!!");

    ConstInfoIteratorT _info = mInfoTab.find(key);
    RUNTIME_ASSERT(_info != mInfoTab.end(), "Can not find the string key!!");
    return _info->second.stringLength;
}


const char * StaticStringDepot::data(
    const StringKeyTypeT key) const
{
    RUNTIME_ASSERT(key != INVALID_STRING_KEY, "Invalid string key!!");

    ConstInfoIteratorT _info = mInfoTab.find(key);
    RUNTIME_ASSERT(_info != mInfoTab.end(), "Can not find the string key!!");
    return (const char*)_info->second.startAddr;
}


StringKeyTypeT StaticStringDepot::cacheString(
    const char * const string)
{
    const StringKeyTypeT _data_key = RUNTIME_HASH(STATIC_STRING_SEED, string);

    if (mInfoTab.find(_data_key) == mInfoTab.end())
    {
        const uint16_t _str_length = (uint16_t)std::strlen(string);
        uint8_t* const _string_addr = mAllocator.allocate(_str_length + 1);
        std::memcpy(_string_addr, string, _str_length);
        _string_addr[_str_length] = 0;

        StringDataInfo & _new_info = mInfoTab[_data_key];
        _new_info.startAddr    = _string_addr;
        _new_info.stringLength = _str_length;
    }

    return _data_key;
}


StaticStringDepot::StaticStringDepot()
{
    static constexpr uint32_t DEPOT_BYTE_SIZE = 8 * 1024 * 1024;

    mAllocator.initialize(MatheHelper::divideUp(DEPOT_BYTE_SIZE, MemoryHelper::pageSize()), 2);

    uint8_t* const _empty_string_addr = mAllocator.allocate(1);
    *_empty_string_addr = 0;
    StringDataInfo & _new_info = mInfoTab[EMPTY_STRING_ID.key];
    _new_info.startAddr    = _empty_string_addr;
    _new_info.stringLength = 0;
}


StaticStringDepot::~StaticStringDepot()
{
    mInfoTab.clear();
    mAllocator.release();
}
