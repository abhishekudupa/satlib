#include <TseitinTransformFull.hpp>
#include <ExpressionBase.hpp>
#include <Literal.hpp>
#include <UIDGenerator.hpp>
#include <SatLibExceptions.hpp>

namespace SatLib {

    extern UIDGenerator AuxUIDGenerator;

    TseitinTransformFull::TseitinTransformFull()
        : CNFConverterBase("TseitinTransformFull")
    {
        ExpCache.set_empty_key(NULL);
        ExpCache.resize(1024);
        LiteralStack.reserve(256);
    }

    TseitinTransformFull::~TseitinTransformFull()
    {
        ExpCache.clear();
    }

    inline void TseitinTransformFull::TransformVar(const ExpressionBase* Exp,
                                            vector<Clause>& ClauseVec)
    {
        LiteralStack.push_back(Literal(Exp->GetVarID(), Exp->IsNegated()));
    }

    inline void TseitinTransformFull::TransformConjunction(u32 NumChildren,
                                                           vector<Clause>& ClauseVec)
    {
        // introduce a new AUX var
        // AUX ==> /\ Children <==> \neg AUX \/ (/\ Children)
        // /\ Children ==> AUX <==> \neg (/\ Children) \/ AUX <==> (\/ \neg Child \/ Aux)
        // Polarity optimization renders the second part unnecessary
        u32 AuxID = (u32)AuxUIDGenerator.GetUID();
        
        Literal AuxLitT(AuxID);
        Literal AuxLitF(AuxID, true);
        
        const u32 ClauseOffset = ClauseVec.size();
        ClauseVec.insert(ClauseVec.end(), move(Clause(NumChildren + 1)));
        // ClauseVec.insert(ClauseVec.end(), NumChildren, Clause(2));
        ClauseVec.reserve(ClauseVec.size() + NumChildren);
        for (u32 i = 0; i < NumChildren; ++i) {
            auto const& CurLit = LiteralStack.back();
            ClauseVec[ClauseOffset][i] = CurLit.Negate();
            Clause CurClause = { AuxLitF, CurLit };
            ClauseVec.push_back(move(CurClause));
            // ClauseVec[ClauseOffset + i + 1][0] = AuxLitF;
            // ClauseVec[ClauseOffset + i + 1][1] = CurLit;
            LiteralStack.pop_back();
        }
        ClauseVec[ClauseOffset][NumChildren] = AuxLitT;
        LiteralStack.push_back(AuxLitT);

        return;
    }

    inline void TseitinTransformFull::TransformNegatedConjunction(u32 NumChildren,
                                                                  vector<Clause>& ClauseVec)
    {
        // AUX <==> \neg (/\ Children) <==> \neg Aux <==> (/\ Children)
        // call TransformConjunction and then flip the literal at the
        // top of stack
        TransformConjunction(NumChildren, ClauseVec);
        LiteralStack[LiteralStack.size() - 1] = LiteralStack.back().Negate();
    }

    inline void TseitinTransformFull::TransformDisjunction(u32 NumChildren, vector<Clause>& ClauseVec)
    {
        // AUX ==> (\/ Children) <==> \neg AUX \/ (\/ Children)
        // (\/ Children) ==> AUX <==> \neg (\/ Children) \/ AUX
        //                       <==> (/\ \neg Child) \/ AUX
        // Polarity optimizations render the second part unnecessary
        u32 AuxID = (u32)AuxUIDGenerator.GetUID();
        
        Literal AuxLitT(AuxID);
        Literal AuxLitF(AuxID, true);
        
        const u32 ClauseOffset = ClauseVec.size();
        ClauseVec.insert(ClauseVec.end(), move(Clause(NumChildren + 1)));
        // ClauseVec.insert(ClauseVec.end(), NumChildren, Clause(2));
        ClauseVec.reserve(ClauseVec.size() + NumChildren);
        for(u32 i = 0; i < NumChildren; ++i) {
            auto const& CurLit = LiteralStack.back();
            ClauseVec[ClauseOffset][i] = CurLit;
            Clause CurClause = { AuxLitT, CurLit.Negate() };
            ClauseVec.push_back(move(CurClause));
            LiteralStack.pop_back();
        }
        ClauseVec[ClauseOffset][NumChildren] = AuxLitF;
        LiteralStack.push_back(AuxLitT);
    }

    inline void TseitinTransformFull::TransformNegatedDisjunction(u32 NumChildren, 
                                                                  vector<Clause>& ClauseVec)
    {
        // Similar to TransformNegatedConjunction
        TransformDisjunction(NumChildren, ClauseVec);
        LiteralStack[LiteralStack.size() - 1] = LiteralStack.back().Negate();
    }

    void TseitinTransformFull::TransformAnd(const ExpressionBase* Exp,
                                            vector<Clause>& ClauseVec)
    {
        // recurse on the children
        auto const& Children = Exp->GetChildren();
        const u32 NumChildren = Children.size();

        for (auto const& Child : Children) {
            TransformSubformula(Child, ClauseVec);
        }

        if (Exp->IsNegated()) {
            TransformNegatedConjunction(NumChildren, ClauseVec);
        } else {
            TransformConjunction(NumChildren, ClauseVec);
        }
    }

    void TseitinTransformFull::TransformOr(const ExpressionBase* Exp,
                                           vector<Clause>& ClauseVec)
    {
        // recurse on the children
        auto const& Children = Exp->GetChildren();
        const u32 NumChildren = Children.size();

        for (auto const& Child : Children) {
            TransformSubformula(Child, ClauseVec);
        }

        if (Exp->IsNegated()) {
            TransformNegatedDisjunction(NumChildren, ClauseVec);
        } else {
            TransformDisjunction(NumChildren, ClauseVec);
        }
    }

    void TseitinTransformFull::TransformXor(const ExpressionBase* Exp,
                                            vector<Clause>& ClauseVec)
    {
        // A xor B <==> (\neg A /\ B) \/ (A /\ \neg B)
        // AUX <==> A xor B 
        // AUX ==> ((!A & B) | (A & !B)) & ((!A & B) | (A & !B)) ==> AUX
        // (!AUX | ((!A & B) | (A & !B))) & (!((!A & B) | (A & !B)) | AUX)
        // We transform each of the subformulae
        // And then push (\neg A /\ B) on stack and call TransformConjunction
        // then push (A /\ \negB) on stack and call TransformConjunction
        // then we simply call TransformDisjunction

        auto const& Children = Exp->GetChildren();
        if (Children.size() != 2) {
            throw InternalError((string)"Internal Error: Expected XOR to have only two children in " + 
                                "TseitinTransformFull :-(");
        }
        TransformSubformula(Children[0], ClauseVec);
        TransformSubformula(Children[1], ClauseVec);
        
        auto ALit = LiteralStack.back();
        auto NegALit = ALit.Negate();
        LiteralStack.pop_back();
        auto BLit = LiteralStack.back();
        auto NegBLit = BLit.Negate();
        LiteralStack.pop_back();

        LiteralStack.push_back(NegALit);
        LiteralStack.push_back(BLit);
        TransformConjunction(2, ClauseVec);
        // auto Term1 = LiteralStack.back();
        // LiteralStack.pop_back();

        LiteralStack.push_back(NegBLit);
        LiteralStack.push_back(ALit);
        TransformConjunction(2, ClauseVec);

        // LiteralStack.push_back(Term1);
        TransformDisjunction(2, ClauseVec);
        if (Exp->IsNegated()) {
            LiteralStack[LiteralStack.size() - 1] = LiteralStack.back().Negate();
        }
        return;
    }

    void TseitinTransformFull::TransformXnor(const ExpressionBase* Exp,
                                             vector<Clause>& ClauseVec)
    {
        // XOR followed by a negation. Just flip
        TransformXor(Exp, ClauseVec);
        LiteralStack[LiteralStack.size() - 1] = LiteralStack.back().Negate();
    }
    
    void TseitinTransformFull::TransformNor(const ExpressionBase* Exp,
                                            vector<Clause>& ClauseVec)
    {
        // Or followed by not. Flip
        TransformOr(Exp, ClauseVec);
        LiteralStack[LiteralStack.size() - 1] = LiteralStack.back().Negate();
    }

    void TseitinTransformFull::TransformNand(const ExpressionBase* Exp,
                                             vector<Clause>& ClauseVec)
    {
        // And followed by not. Flip
        TransformAnd(Exp, ClauseVec);
        LiteralStack[LiteralStack.size() - 1] = LiteralStack.back().Negate();
    }

    void TseitinTransformFull::TransformImplies(const ExpressionBase* Exp,
                                                vector<Clause>& ClauseVec)
    {
        // AUX ==> (A ==> B) <==> \neg AUX \/ \neg A \/ B
        // (A ==> B) ==> AUX <==> (\neg (\neg A \/ B)) \/ AUX
        //                   <==> (A /\ \neg B) \/ AUX
        //                   <==> (A \/ AUX) /\ (\neg B \/ AUX)

        u32 AuxID = (u32)AuxUIDGenerator.GetUID();
        Literal AuxLitT(AuxID);
        Literal AuxLitF(AuxID, true);

        auto const& Children = Exp->GetChildren();
        if (Children.size() != 2) {
            throw InternalError((string)"Internal Error: Expected Implies expression to have only 2 children " +
                                "in TseitinTransformFull :-(");
        }

        TransformSubformula(Children[0], ClauseVec);
        TransformSubformula(Children[1], ClauseVec);
        
        auto LitB = LiteralStack.back();
        LiteralStack.pop_back();
        auto LitA = LiteralStack.back();
        LiteralStack.pop_back();
        
        Clause Clause1(3);
        Clause1[0] = AuxLitF;
        Clause1[1] = LitA.Negate();
        Clause1[2] = LitB;

        Clause Clause2(2);
        Clause2[0] = LitA;
        Clause2[1] = AuxLitT;
        
        Clause Clause3(2);
        Clause3[0] = LitB.Negate();
        Clause3[1] = AuxLitT;

        ClauseVec.push_back(move(Clause1));
        ClauseVec.push_back(move(Clause2));
        ClauseVec.push_back(move(Clause3));
        
        if (Exp->IsNegated()) {
            LiteralStack.push_back(AuxLitF);
        } else {
            LiteralStack.push_back(AuxLitT);
        }
    }

    void TseitinTransformFull::TransformIff(const ExpressionBase* Exp,
                                            vector<Clause>& ClauseVec)
    {
        TransformXnor(Exp, ClauseVec);
    }

    void TseitinTransformFull::TransformSubformula(const ExpressionBase* Exp,
                                                   vector<Clause>& ClauseVec)
    {
        // Check if we have a cached literal
        auto it = ExpCache.find(Exp);
        if (it != ExpCache.end()) {
            LiteralStack.push_back(it->second);
            return;
        }

        switch(Exp->GetOp()) {
        case EXP_AND:
            TransformAnd(Exp, ClauseVec);
            break;
        case EXP_OR:
            TransformOr(Exp, ClauseVec);
            break;
        case EXP_XOR:
            TransformXor(Exp, ClauseVec);
            break;
        case EXP_XNOR:
            TransformXnor(Exp, ClauseVec);
            break;
        case EXP_NAND:
            TransformNand(Exp, ClauseVec);
            break;
        case EXP_NOR:
            TransformNor(Exp, ClauseVec);
            break;
        case EXP_VAR:
            TransformVar(Exp, ClauseVec);
            break;
        case EXP_IMPLIES:
            TransformImplies(Exp, ClauseVec);
            break;
        case EXP_IFF:
            TransformIff(Exp, ClauseVec);
            break;
        case EXP_TRUE:
        case EXP_FALSE:
            // Should never happen
            throw InternalError((string)"Internal Error: Did not expect constant true/false expression in " + 
                                "TseitinTransformFull :-(");
            break; // ?

        default:
            throw InternalError((string)"Internal Error: Encountered unexpected value for operator in " + 
                                "expression during TseitinTransformFull :-(");
        }

        // Cache the Literal for the future
        ExpCache[Exp] = LiteralStack.back();
    }

    void TseitinTransformFull::operator () (const ExpressionBase* Exp,
                                            vector<Clause>& ClauseVec)
    {
        LiteralStack.clear();
        ClauseVec.clear();

        if (Exp->GetOp() == EXP_TRUE) {
            // Just introduce a new aux var
            ClauseVec.push_back(Clause(1, Literal((u32)AuxUIDGenerator.GetUID(), false)));
            return;
        } else if (Exp->GetOp() == EXP_FALSE) {
            // Introduce new aux var and assert aux & !aux
            auto MyLit = Literal((u32)AuxUIDGenerator.GetUID(), false);
            auto MyNegLit = MyLit.Negate();
            ClauseVec.push_back(Clause(1, MyLit));
            ClauseVec.push_back(Clause(1, MyNegLit));
            return;
        } else {
            TransformSubformula(Exp, ClauseVec);
            if (LiteralStack.size() != 1) {
                cout << LiteralStack.size() << endl;
                throw InternalError((string)"Internal Error: Expected only one literal to remain on stack " + 
                                    "after TseitinTransformFull :-(");
            }
            ClauseVec.push_back(Clause(1, LiteralStack.back()));
            LiteralStack.pop_back();
            return;
        }
    }

    void TseitinTransformFull::Reset()
    {
        LiteralStack.clear();
        ExpCache.clear();
    }

} /* end namespace */

