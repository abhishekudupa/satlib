#if !defined __SATLIB_UID_GENERATOR_HPP
#define __SATLIB_UID_GENERATOR_HPP

#include "SatLibCommon.hpp"

namespace SatLib {

    class UIDGenerator
    {
      private:
        u64 NextID;
        u64 StartOffset;

      public:
        UIDGenerator();
        UIDGenerator(u64 StartOffset);
        u64 GetUID();
        string GetStringUID(u32 PadDigits = 16);
        string GetStringUID(const string& Prefix, u32 PadDigits = 16);
        void Reset();
    };

} /* End namespace */

#endif /* __ESOLVER_UID_GENERATOR_HPP */

