#include "assert/runtime-assert.h"
#include "text/static-string-depot.h"
#include "text/static-string.h"


StaticStringIdT StaticString::getEmptyStringId()
{
    return StaticStringDepot::ref().getEmptyStringId();
}


StaticString::StaticString()
:
    mStringId(getEmptyStringId())
#if (BUILD_MODE == DEBUG_BUILD_MODE)
    ,mStoredString(data())
#endif
{
    static_assert(sizeof(StaticString::mStringId) == sizeof(StringKeyTypeT), "");
}


StaticString::StaticString(
    const StaticStringIdT id)
:
    mStringId(id)
#if (BUILD_MODE == DEBUG_BUILD_MODE)
    , mStoredString(data())
#endif
{
    RUNTIME_ASSERT(id != INVALID_STATIC_STRING_ID, "Invalid string Id!!");
}


StaticString::StaticString(
    const char* const string)
:
    mStringId({ StaticStringDepot::ref().cacheString(string) })
#if (BUILD_MODE == DEBUG_BUILD_MODE)
    , mStoredString(data())
#endif
{

}


StaticStringIdT StaticString::id() const
{
    return mStringId;
}


uint16_t StaticString::length() const
{
    return StaticStringDepot::ref().length(mStringId.key);
}


const char* StaticString::data() const
{
    return StaticStringDepot::ref().data(mStringId.key);
}


bool StaticString::operator==(
    const StaticString other) const
{
    return mStringId == other.mStringId;
}


bool StaticString::operator!=(
    const StaticString other) const
{
    return mStringId != other.mStringId;
}
