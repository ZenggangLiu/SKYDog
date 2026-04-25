/// Library headers
#include "Hashing/RuntimeXXHash32.hpp"
#include "Hashing/RuntimeXXHash64.hpp"
/// Self header
#include "Hashing/RuntimeHash.hpp"


uint32_t
RuntimeHash::hash32 (
    const uint32_t        seed,
    const uint8_t * const data,
    const uint64_t        length)
{
    return XXHash32::hash(seed, data, length);
}


uint64_t
RuntimeHash::hash64 (
    const uint32_t       seed,
    const uint8_t* const data,
    const uint64_t       length)
{
    return XXHash64::hash(seed, data, length);
}
