#include <CNFConverterBase.hpp>

namespace SatLib {

    CNFConverterBase::CNFConverterBase(const string& ConverterName)
        : ConverterName(ConverterName)
    {
        // Nothing here
    }

    CNFConverterBase::~CNFConverterBase()
    {
        // Nothing here
    }

    const string& CNFConverterBase::GetName() const
    {
        return ConverterName;
    }


} /* End namespace */

