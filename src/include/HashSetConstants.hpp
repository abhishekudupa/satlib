#if !defined __SATLIB_HASH_SET_CONSTANTS_HPP
#define __SATLIB_HASH_SET_CONSTANTS_HPP

#include "SatLibCommon.hpp"

#define INIT_HASH_TABLE_INDEX (0)
// Index: 0 ==> 2^5 buckets
// Index: 1 ==> 2^6 buckets and so on
#define LIMIT_HASH_TABLE_TO_INDEX (18)
// Rehash when load factor >= DEFAULT_REHASH_TRIGGER
#define DEFAULT_REHASH_TRIGGER (0.86667)
// This will change if more primes are added to the prime table
#define MAX_HASH_TABLE_INDEX (26)


namespace SatLib {
    // A table of primes for use with the hash-table
    // Max Index = 26
    static const u32 PrimeTable[] = {
        37, 67, 131, 257, 521, 1031, 2053, 4099, 8209, 
        16411, 32771, 65537, 131101, 262147, 524309, 1048583,
        2097169, 4194319, 8388617, 16777259, 33554467, 
        67108879, 134217757, 268435459, 536870923,
        1073741827, 2147483659
    };

    static const u32 KeepBits[] = {
        6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
        21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32
    };
} /* End namespace */

#endif /* __SATLIB_HASH_SET_CONSTANTS_HPP */

