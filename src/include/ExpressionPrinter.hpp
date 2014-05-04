#if !defined __SATLIB_EXPRESSION_PRINTER_HPP
#define __SATLIB_EXPRESSION_PRINTER_HPP

#include "SatLibForwardDecls.hpp"
#include "ExpressionVisitorBase.hpp"

namespace SatLib {

    class ExpressionPrinter : public ExpressionVisitorBase
    {
        ostream& Str;
    public:
        ExpressionPrinter(ostream& Str);
        virtual ~ExpressionPrinter();
        
        virtual void PreVisit(const ExpressionBase* Exp);
        virtual void Visit(const ExpressionBase* Exp, u32 VisitNum);
        virtual void PostVisit(const ExpressionBase* Exp);
    };

} /* End namespace */

#endif /* __SATLIB_EXPRESSION_PRINTER_HPP */

