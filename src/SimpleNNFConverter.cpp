#include <SimpleNNFConverter.hpp>
#include <ExpressionBase.hpp>
#include <ExpressionManager.hpp>
#include <Utils.hpp>

namespace SatLib {

    SimpleNNFConverter::SimpleNNFConverter()
        : NNFConverterBase("SimpleNNFConverter")
    {
        // Nothing here
    }

    SimpleNNFConverter::~SimpleNNFConverter()
    {
        // Nothing here
    }

    // Core implementation
    
    inline const ExpressionBase* SimpleNNFConverter::NormalizeXOR(ExpressionManager* Mgr, 
                                                                  const ExpressionBase* Exp)
    {
        // XORs are always binary
        const vector<const ExpressionBase*>& Children = Exp->GetChildren();
        assert(Children.size() == 2);
                
        const ExpressionBase* Retval = Mgr->CreateOr(Mgr->CreateAnd(Children[0], 
                                                                    Mgr->CreateNot(Children[1])),
                                                     Mgr->CreateAnd(Mgr->CreateNot(Children[0]), 
                                                                    Children[1]));
        if(Exp->IsNegated()) {
            return Mgr->CreateNot(Retval);
        } else {
            return Retval;
        }
    }

    inline const ExpressionBase* SimpleNNFConverter::NormalizeXNOR(ExpressionManager* Mgr, 
                                                                   const ExpressionBase* Exp)
    {
        // XNORs are always binary
        const vector<const ExpressionBase*>& Children = Exp->GetChildren();
        assert(Children.size() == 2);
        
        
        const ExpressionBase* Retval = 
            Mgr->CreateOr(Mgr->CreateAnd(Children[0], Children[1]),
                          Mgr->CreateAnd(Mgr->CreateNot(Children[0]), Mgr->CreateNot(Children[1])));
        if(Exp->IsNegated()) {
            return Mgr->CreateNot(Retval);
        } else {
            return Retval;
        }
    }

    inline const ExpressionBase* SimpleNNFConverter::NormalizeNAND(ExpressionManager* Mgr, 
                                                                   const ExpressionBase* Exp)
    {
        const vector<const ExpressionBase*>& Children = Exp->GetChildren();

        const ExpressionBase* Retval = Mgr->CreateNot(Mgr->CreateAnd(Children));
        if(Exp->IsNegated()) {
            return Mgr->CreateNot(Retval);
        } else {
            return Retval;
        }
    }

    inline const ExpressionBase* SimpleNNFConverter::NormalizeNOR(ExpressionManager* Mgr, 
                                                                  const ExpressionBase* Exp)
    {
        const vector<const ExpressionBase*>& Children = Exp->GetChildren();

        const ExpressionBase* Retval = Mgr->CreateNot(Mgr->CreateOr(Children));
        if(Exp->IsNegated()) {
            return Mgr->CreateNot(Retval);
        } else {
            return Retval;
        }
    }

    inline const ExpressionBase* SimpleNNFConverter::NormalizeImplies(ExpressionManager* Mgr, 
                                                                      const ExpressionBase* Exp)
    {
        // Implies is binary
        const vector<const ExpressionBase*>& Children = Exp->GetChildren();
        assert(Children.size() == 2);
                
        const ExpressionBase* Retval = Mgr->CreateOr(Mgr->CreateNot(Children[0]), Children[1]);
        if(Exp->IsNegated()) {
            return Mgr->CreateNot(Retval);
        } else {
            return Retval;
        }
    }

    inline const ExpressionBase* SimpleNNFConverter::NormalizeIff(ExpressionManager* Mgr, 
                                                                  const ExpressionBase* Exp)
    {
        // Iff is binary
        const vector<const ExpressionBase*>& Children = Exp->GetChildren();
        assert(Children.size() == 2);
        
        const ExpressionBase* Retval = Mgr->CreateAnd(Mgr->CreateOr(Mgr->CreateNot(Children[0]), 
                                                                    Children[1]),
                                                      Mgr->CreateOr(Mgr->CreateNot(Children[1]), 
                                                                    Children[0]));
        if(Exp->IsNegated()) {
            return Mgr->CreateNot(Retval);
        } else {
            return Retval;
        }
    }

    inline const ExpressionBase* SimpleNNFConverter::PushNegations(ExpressionManager* Mgr,
                                                                   const ExpressionBase* Exp)
    {
        if(Exp->IsNegated()) {
            switch(Exp->GetOp()) {
            case EXP_VAR:
                return Exp;
            case EXP_AND:
            case EXP_OR: {
                vector<const ExpressionBase*> PushedChildren;
                const vector<const ExpressionBase*>& Children = Exp->GetChildren();
                const u32 NumChildren = Children.size();
                for(u32 i = 0; i < NumChildren; ++i) {
                    PushedChildren.push_back(PushNegations(Mgr, Mgr->CreateNot(Children[i])));
                }
                return Mgr->CreateOp((Exp->GetOp() == EXP_AND ? EXP_OR : EXP_AND), PushedChildren);
            } break;
                
            default:
                assert(false);
            }
        } else {
            switch(Exp->GetOp()) {
            case EXP_VAR:
                return Exp;
            case EXP_AND:
            case EXP_OR: {
                const vector<const ExpressionBase*>& Children = Exp->GetChildren();
                vector<const ExpressionBase*> PushedChildren;
                const u32 NumChildren = Children.size();
                for(u32 i = 0; i < NumChildren; ++i) {
                    PushedChildren.push_back(PushNegations(Mgr, Children[i]));
                }
                return Mgr->CreateOp(Exp->GetOp(), PushedChildren);
            } break;
                
            default:
                assert(false);
            }
        }
    }

    inline const ExpressionBase* SimpleNNFConverter::NormalizeExp(ExpressionManager* Mgr, 
                                                                  const ExpressionBase* Exp)
    {
        vector<const ExpressionBase*> NewChildren;

        switch(Exp->GetOp()) {
        case EXP_TRUE:
        case EXP_FALSE:
        case EXP_VAR:
            return Exp;

        case EXP_XOR:
            return NormalizeXOR(Mgr, Exp);
        case EXP_XNOR:
            return NormalizeXNOR(Mgr, Exp);
        case EXP_NAND:
            return NormalizeNAND(Mgr, Exp);
        case EXP_NOR:
            return NormalizeNOR(Mgr, Exp);
        case EXP_IMPLIES:
            return NormalizeImplies(Mgr, Exp);
        case EXP_IFF:
            return NormalizeIff(Mgr, Exp);
        case EXP_AND:
        case EXP_OR: 
            {
                const vector<const ExpressionBase*>& Children = Exp->GetChildren();
                const u32 NumChildren = Children.size();
                for(u32 i = 0; i < NumChildren; ++i) {
                    NewChildren.push_back(NormalizeExp(Mgr, Children[i]));
                }
                if(Exp->GetOp() == EXP_AND) {
                    return (Exp->IsNegated() ? Mgr->CreateNot(Mgr->CreateAnd(NewChildren)) :
                            Mgr->CreateAnd(NewChildren));
                } else {
                    return (Exp->IsNegated() ? Mgr->CreateNot(Mgr->CreateOr(NewChildren)) :
                            Mgr->CreateOr(NewChildren));
                }
            }
        default:
            assert(false);
        }
    }

    const ExpressionBase* SimpleNNFConverter::Convert(ExpressionManager* Mgr, const ExpressionBase* Exp)
    {
        const ExpressionBase* ActExp = TryMigrate(Mgr, Exp);

        // Reduce to AND-OR-NOT form first
        const ExpressionBase* AndOrNotExp = NormalizeExp(Mgr, ActExp);
        return PushNegations(Mgr, AndOrNotExp);

        switch(Exp->GetOp()) {
        case EXP_VAR:
        case EXP_TRUE:
        case EXP_FALSE:
            return TryMigrate(Mgr, Exp);
        case EXP_AND:
        case EXP_OR:
            {
                return PushNegations(Mgr, Exp);
            }

        case EXP_XOR:
            return Convert(Mgr, NormalizeXOR(Mgr, Exp));
        case EXP_XNOR:
            return Convert(Mgr, NormalizeXNOR(Mgr, Exp));
        case EXP_NAND:
            return Convert(Mgr, NormalizeNAND(Mgr, Exp));
        case EXP_NOR:
            return Convert(Mgr, NormalizeNOR(Mgr, Exp));
        case EXP_IMPLIES:
            return Convert(Mgr, NormalizeImplies(Mgr, Exp));
        case EXP_IFF:
            return Convert(Mgr, NormalizeImplies(Mgr, Exp));
        }
        return NULL;
    }

    const ExpressionBase* SimpleNNFConverter::operator () (ExpressionManager* Mgr, const ExpressionBase* Exp)
    {
        return Convert(Mgr, Exp);
    }

} /* End namespace */

