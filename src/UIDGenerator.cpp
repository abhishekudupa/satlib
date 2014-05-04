#include <UIDGenerator.hpp>
#include <sstream>
#include <iomanip>

namespace SatLib {

    UIDGenerator::UIDGenerator()
        : NextID(0), StartOffset(0)
    {
        // Nothing here
    }

    UIDGenerator::UIDGenerator(u64 StartOffset)
        : NextID(StartOffset), StartOffset(StartOffset)
    {
        // Nothing here
    }

    u64 UIDGenerator::GetUID()
    {
        return (NextID++);
    }
    
    void UIDGenerator::Reset()
    {
        NextID = StartOffset;
    }

    string UIDGenerator::GetStringUID(u32 PadDigits)
    {
        u64 UID = GetUID();
        ostringstream sstr;
        sstr << setw(PadDigits) << setfill('0') << UID;
        return sstr.str();
    }

    string UIDGenerator::GetStringUID(const string& Prefix, u32 PadDigits)
    {
        u64 UID = GetUID();
        ostringstream sstr;
        sstr << Prefix << setw(PadDigits) << setfill('0') << UID;
        return sstr.str();
    }

} /* End namespace */

