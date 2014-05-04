#if !defined __SATLIB_EXPRESSION_VISITOR_BASE_HPP
#define __SATLIB_EXPRESSION_VISITOR_BASE_HPP

#include "SatLibForwardDecls.hpp"

namespace SatLib {

    class ExpressionVisitorBase
    {
    private:
        string VisitorName;

    public:
        ExpressionVisitorBase(const string& VisitorName);
        virtual ~ExpressionVisitorBase();

        const string& GetName() const;
        virtual void PreVisit(const ExpressionBase* Exp) = 0;
        virtual void Visit(const ExpressionBase* Exp, u32 VisitNum = 0) = 0;
        virtual void PostVisit(const ExpressionBase* Exp) = 0;
    };

} /* End namespace */

#endif /* __SATLIB_EXPRESSION_VISITOR_BASE_HPP */

