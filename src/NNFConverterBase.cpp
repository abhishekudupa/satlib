#include <NNFConverterBase.hpp>

namespace SatLib {

    NNFConverterBase::NNFConverterBase(const string& ConverterName)
        : ConverterName(ConverterName)
    {
        // Nothing here
    }

    NNFConverterBase::~NNFConverterBase()
    {
        // Nothing here
    }

    const string& NNFConverterBase::GetName() const
    {
        return ConverterName;
    }

} /* End namespace */

