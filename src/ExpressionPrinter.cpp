#include <ExpressionPrinter.hpp>
#include <ExpressionBase.hpp>

namespace SatLib {

    ExpressionPrinter::ExpressionPrinter(ostream& Str)
        : ExpressionVisitorBase("ExpressionPrinter"), Str(Str)
    {
        // Nothing here
    }

    ExpressionPrinter::~ExpressionPrinter()
    {
        // Nothing here
    }

    void ExpressionPrinter::PreVisit(const ExpressionBase* Exp)
    {
        if (Exp->IsNegated()) {
            Str << "!";
        }
        if (Exp->GetOp() != EXP_VAR && Exp->GetOp() != EXP_TRUE && Exp->GetOp() != EXP_FALSE) {
            Str << "(";
        }
    }

    void ExpressionPrinter::Visit(const ExpressionBase* Exp, u32 VisitNum)
    {
        switch(Exp->GetOp()) {
        case EXP_VAR:
            Str << Exp->GetVarName(); break;

        case EXP_AND:
            if (VisitNum < Exp->GetChildren().size() - 1) {
                Str << " & ";
            }
            break;

        case EXP_OR:
            if (VisitNum < Exp->GetChildren().size() - 1) {
                Str << " | ";
            }
            break;

        case EXP_NAND:
            if (VisitNum < Exp->GetChildren().size() - 1) {
                Str << " N& ";
            }
            break;
            
        case EXP_NOR:
            if (VisitNum < Exp->GetChildren().size() - 1) {
                Str << " N| ";
            }
            break;

        case EXP_XOR:
            if (VisitNum < Exp->GetChildren().size() - 1) {
                Str << " X| ";
            }
            break;

        case EXP_XNOR:
            if (VisitNum < Exp->GetChildren().size() - 1) {
                Str << " XN| ";
            }
            break;

        case EXP_IMPLIES:
            if (VisitNum < Exp->GetChildren().size() - 1) {
                Str << " -> ";
            }
            break;   
            
        case EXP_IFF:
            if (VisitNum < Exp->GetChildren().size() - 1) {
                Str << " <-> ";
            }
            break;

        case EXP_TRUE:
            Str << "TRUE"; 
            break;

        case EXP_FALSE:
            Str << "FALSE";
            break;
        }
    }

    void ExpressionPrinter::PostVisit(const ExpressionBase* Exp)
    {
        if (Exp->GetOp() != EXP_VAR && Exp->GetOp() != EXP_TRUE && Exp->GetOp() != EXP_FALSE) {
            Str << ")";
        }
    }

} /* End namespace */
