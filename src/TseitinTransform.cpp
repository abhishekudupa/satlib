#include <TseitinTransform.hpp>
#include <UIDGenerator.hpp>
#include <Literal.hpp>
#include <ExpressionBase.hpp>

namespace SatLib {

    UIDGenerator AuxUIDGenerator(SATLIB_AUX_VAR_OFFSET);

    TseitinTransform::TseitinTransform()
        : CNFConverterBase("TseitinTransorm")
    {
        // Nothing here
    }

    TseitinTransform::~TseitinTransform()
    {
        // Nothing here
    }

    // Core implementation
    // Assumption, Exp is in NNF already!

    void TseitinTransform::TransformVar(const ExpressionBase* Exp,
                                        vector<Clause>& ClauseVec)
    {
        LiteralStack.push_back(Literal(Exp->GetVarID(), Exp->IsNegated()));
    }

    void TseitinTransform::TransformOr(const ExpressionBase* Exp,
                                       vector<Clause>& ClauseVec)
    {
        const vector<const ExpressionBase*>& Children = Exp->GetChildren();
        const u32 NumChildren = Children.size();
        u32 MyAuxID = (u32)AuxUIDGenerator.GetUID();
        Literal MyAuxLitT(MyAuxID);
        Literal MyAuxLitF(MyAuxID, true);

        for(u32 i = 0; i < NumChildren; ++i) {
            TransformSubformula(Children[i], ClauseVec);
        }
        // Add clauses for the child subformula from the literal stack
        u32 ClauseOffset = ClauseVec.size();
        ClauseVec.insert(ClauseVec.end(), NumChildren + 1, Clause());
        ClauseVec[ClauseOffset].push_back(MyAuxLitF);

        for(u32 i = 0; i < NumChildren; ++i) {
            ClauseVec[ClauseOffset].push_back(LiteralStack.back());
            ClauseVec[ClauseOffset + i + 1].push_back(MyAuxLitT);
            ClauseVec[ClauseOffset + i + 1].push_back(LiteralStack.back().Negate());
            LiteralStack.pop_back();
        }

        LiteralStack.push_back(MyAuxLitT);
        return;
    }

    void TseitinTransform::TransformAnd(const ExpressionBase* Exp,
                                        vector<Clause>& ClauseVec)
    {
        const vector<const ExpressionBase*>& Children = Exp->GetChildren();
        const u32 NumChildren = Children.size();
        auto MyAuxID = (u32)AuxUIDGenerator.GetUID();
        Literal MyAuxLitT(MyAuxID);
        Literal MyAuxLitF(MyAuxID, true);

        for(u32 i = 0; i < NumChildren; ++i) {
            TransformSubformula(Children[i], ClauseVec);
        }

        // Add clauses for the child formula from the literal stack
        u32 ClauseOffset = ClauseVec.size();
        ClauseVec.insert(ClauseVec.end(), NumChildren + 1, Clause());
        ClauseVec[ClauseOffset].push_back(MyAuxLitT);
        
        for(u32 i = 0; i < NumChildren; ++i) {
            ClauseVec[ClauseOffset].push_back(LiteralStack.back().Negate());
            ClauseVec[ClauseOffset + i + 1].push_back(MyAuxLitF);
            ClauseVec[ClauseOffset + i + 1].push_back(LiteralStack.back());
            LiteralStack.pop_back();
        }
        
        LiteralStack.push_back(MyAuxLitT);
        return;
    }

    void TseitinTransform::TransformSubformula(const ExpressionBase* Exp,
                                               vector<Clause>& ClauseVec)
    {
        switch(Exp->GetOp()) {
        case EXP_VAR:
            return TransformVar(Exp, ClauseVec);
        case EXP_AND:
            return TransformAnd(Exp, ClauseVec);
        case EXP_OR:
            return TransformOr(Exp, ClauseVec);
        default:
            assert(false);
        }
    }

    void TseitinTransform::operator () (const ExpressionBase* Exp,
                                        vector<Clause>& ClauseVec)
    {
        // ClauseVec.clear();
        LiteralStack.clear();
        TransformSubformula(Exp, ClauseVec);
        assert(LiteralStack.size() == 1);
        ClauseVec.push_back(Clause(1, LiteralStack.back()));
        LiteralStack.pop_back();
        return;
    }

    void TseitinTransform::Reset()
    {
        LiteralStack.clear();
    }

} /* End namespace */

