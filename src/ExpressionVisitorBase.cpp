#include <ExpressionVisitorBase.hpp>

namespace SatLib {

    ExpressionVisitorBase::ExpressionVisitorBase(const string& VisitorName)
        : VisitorName(VisitorName)
    {
        // Nothing here
    }

    ExpressionVisitorBase::~ExpressionVisitorBase()
    {
        // Nothing here
    }

    const string& ExpressionVisitorBase::GetName() const
    {
        return VisitorName;
    }

} /* End namespace */
