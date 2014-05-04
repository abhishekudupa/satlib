#include <ExpressionManager.hpp>
#include <ExpressionBase.hpp>
#include <SatLibExceptions.hpp>

namespace SatLib {

    const string ExpressionManager::EmptyString = "";

    inline void ExpressionManager::ClearScope(ExpHashSet* Scope) const
    {
        for(auto const& Elem : *Scope) {
            delete Elem;
        }
    }

    inline ExpressionBase* ExpressionManager::ScopedLookup(ExpressionBase* Exp) const
    {
        const u32 NumScopes = ScopeStack.size();

        for(i32 i = NumScopes - 1; i >= 0; --i) {
            auto it = ScopeStack[i]->find(Exp);
            if (it != ScopeStack[i]->end()) {
                return *it;
            }
        }

        return NULL;
    }

    inline u64 ExpressionManager::ScopedLookup(const string& VarName) const
    {
        const u32 NumScopes = VarNameToVarIDMapStack.size();
        for(i32 i = NumScopes - 1; i >= 0; --i) {
            auto it = VarNameToVarIDMapStack[i].find(VarName);
            if (it != VarNameToVarIDMapStack[i].end()) {
                return it->second;
            }
        }
        return 0;
    }

    inline const string& ExpressionManager::ScopedLookup(u64 VarID) const
    {
        const u32 NumScopes = VarIDToVarNameMapStack.size();
        for(i32 i = NumScopes - 1; i >= 0; --i) {
            auto it = VarIDToVarNameMapStack[i].find(VarID);
            if (it != VarIDToVarNameMapStack[i].end()) {
                return it->second;
            }
        }
        return EmptyString;
    }

    const ExpressionBase* ExpressionManager::CreateOp(ExpOperator Op, 
                                                      const vector<const ExpressionBase*>& Args)
    {
        ExpressionBase* Retval = new ExpressionBase(const_cast<ExpressionManager*>(this), Op, Args);
        ExpressionBase* Cached = ScopedLookup(Retval);
        if(Cached != NULL) {
            delete Retval;
            return Cached;
        }
        ScopeStack.back()->insert(Retval);
        return Retval;
    }

    const ExpressionBase* ExpressionManager::CreateOp(ExpOperator Op,
                                                      const ExpressionBase* Arg1,
                                                      const ExpressionBase* Arg2)
    {
        vector<const ExpressionBase*> Args;
        Args.push_back(Arg1);
        Args.push_back(Arg2);
        return CreateOp(Op, Args);
    }

    const ExpressionBase* ExpressionManager::CreateTrue()
    {
        return TrueExp;
    }

    const ExpressionBase* ExpressionManager::CreateFalse()
    {
        return FalseExp;
    }

    inline void ExpressionManager::CheckMgr(const ExpressionBase* Exp) const
    {
        if (Exp->GetMgr() != this) {
            throw ManagerMismatchException(Exp);
        }
    }

    inline void ExpressionManager::CheckMgr(const vector<const ExpressionBase*>& Exps) const
    {
        const u32 NumExps = Exps.size();
        for(u32 i = 0; i < NumExps; ++i) {
            CheckMgr(Exps[i]);
        }
    }

    inline bool ExpressionManager::CheckTrue(const vector<const ExpressionBase*>& Exps) const
    {
        const u32 NumExps = Exps.size();
        for(u32 i = 0; i < NumExps; ++i) {
            if(Exps[i] == TrueExp) {
                return true;
            }
        }
        return false;
    }

    inline bool ExpressionManager::CheckFalse(const vector<const ExpressionBase*>& Exps) const
    {
        const u32 NumExps = Exps.size();
        for(u32 i = 0; i < NumExps; ++i) {
            if(Exps[i] == FalseExp) {
                return true;
            }
        }
        return false;
    }

    inline vector<const ExpressionBase*> 
    ExpressionManager::EliminateTrue(const vector<const ExpressionBase*>& Exps) const
    {
        vector<const ExpressionBase*> Retval;
        const u32 NumExps = Exps.size();
        for(u32 i = 0; i < NumExps; ++i) {
            if(Exps[i] != TrueExp) {
                Retval.push_back(Exps[i]);
            }
        }
        return Retval;
    }

    inline vector<const ExpressionBase*> 
    ExpressionManager::EliminateFalse(const vector<const ExpressionBase*>& Exps) const
    {
        vector<const ExpressionBase*> Retval;
        const u32 NumExps = Exps.size();
        for(u32 i = 0; i < NumExps; ++i) {
            if(Exps[i] != FalseExp) {
                Retval.push_back(Exps[i]);
            }
        }
        return Retval;
    }

    ExpressionManager::ExpressionManager()
        : VarUIDGenerator(2)
    {
        // Push a default scope
        ScopeStack.push_back(new ExpHashSet());
        ScopeStack.back()->set_empty_key(NULL);
        VarIDToVarNameMapStack.push_back(unordered_map<u64, string>());
        VarNameToVarIDMapStack.push_back(unordered_map<string, u64>());

        // Add the constant true and false expressions
        TrueExp = new ExpressionBase(this, true);
        FalseExp = new ExpressionBase(this, false);
        ScopeStack.back()->insert(const_cast<ExpressionBase*>(TrueExp));
        ScopeStack.back()->insert(const_cast<ExpressionBase*>(FalseExp));
    }

    ExpressionManager::~ExpressionManager()
    {
        // Delete all the expressions in all scopes
        while(ScopeStack.size() > 0) {
            ClearScope(ScopeStack.back());
            delete ScopeStack.back();
            ScopeStack.pop_back();
        }
        VarIDToVarNameMapStack.clear();
        VarNameToVarIDMapStack.clear();
    }

    void ExpressionManager::Push()
    {
        ScopeStack.push_back(new ExpHashSet());
        ScopeStack.back()->set_empty_key(NULL);
        VarIDToVarNameMapStack.push_back(unordered_map<u64, string>());
        VarNameToVarIDMapStack.push_back(unordered_map<string, u64>());
    }

    void ExpressionManager::Pop(u32 NumScopes)
    {
        for(u32 i = 0; i < NumScopes; ++i) {
            ClearScope(ScopeStack.back());
            delete ScopeStack.back();
            ScopeStack.pop_back();
            VarIDToVarNameMapStack.back().clear();
            VarIDToVarNameMapStack.pop_back();
            VarNameToVarIDMapStack.back().clear();
            VarNameToVarIDMapStack.pop_back();
        }
    }

    const ExpressionBase* ExpressionManager::CreateAnd(const vector<const ExpressionBase*>& Args)
    {
        CheckMgr(Args);
        if(CheckFalse(Args)) {
            return FalseExp;
        }
        vector<const ExpressionBase*> ActArgs = EliminateTrue(Args);
        if(ActArgs.size() == 0) {
            return TrueExp;
        }
        if(ActArgs.size() == 1) {
            return ActArgs[0];
        }
        return CreateOp(EXP_AND, ActArgs);
    }

    const ExpressionBase* ExpressionManager::CreateOr(const vector<const ExpressionBase*>& Args)
    {
        CheckMgr(Args);
        if(CheckTrue(Args)) {
            return TrueExp;
        }
        vector<const ExpressionBase*> ActArgs = EliminateFalse(Args);
        if(ActArgs.size() == 0) {
            return FalseExp;
        }
        if(ActArgs.size() == 1) {
            return ActArgs[0];
        }
        return CreateOp(EXP_OR, ActArgs);
    }

    const ExpressionBase* ExpressionManager::CreateNand(const vector<const ExpressionBase*>& Args)
    {
        CheckMgr(Args);
        if(CheckFalse(Args)) {
            return TrueExp;
        }

        vector<const ExpressionBase*> ActArgs = EliminateTrue(Args);
        if(ActArgs.size() == 0) {
            return FalseExp;
        }
        if(ActArgs.size() == 1) {
            return CreateNot(ActArgs[0]);
        }
        return CreateOp(EXP_NAND, ActArgs);
    }

    const ExpressionBase* ExpressionManager::CreateNor(const vector<const ExpressionBase*>& Args)
    {
        CheckMgr(Args);
        if(CheckTrue(Args)) {
            return FalseExp;
        }

        vector<const ExpressionBase*> ActArgs = EliminateFalse(Args);
        if(ActArgs.size() == 0) {
            return TrueExp;
        }
        if(ActArgs.size() == 1) {
            return CreateNot(ActArgs[0]);
        }
        return CreateOp(EXP_NOR, ActArgs);
    }

    const ExpressionBase* ExpressionManager::CreateNot(const ExpressionBase* Arg)
    {
        ExpressionBase* Retval;
        CheckMgr(Arg);
        if(Arg == TrueExp) {
            return FalseExp;
        } else if(Arg == FalseExp) {
            return TrueExp;
        } else if(Arg->GetOp() == EXP_VAR) {
            Retval = new ExpressionBase(this, Arg->GetVarID(),
                                        !Arg->IsNegated());
        } else {
            Retval = new ExpressionBase(this, Arg->GetOp(),
                                        Arg->GetChildren(), 
                                        !Arg->IsNegated());
        }

        ExpressionBase* Cached = ScopedLookup(Retval);
        if(Cached != NULL) {
            delete Retval;
            return Cached;
        }
        ScopeStack.back()->insert(Retval);
        return Retval;
    }

    const ExpressionBase* ExpressionManager::CreateVar(const string& VarName)
    {
        if (VarName == "") {
            throw InvalidExpressionException((string)"VAR expressions must have a valid name. " +
                                             "The empty string is not a valid name.");
        }
        auto VarID = ScopedLookup(VarName);
        if(VarID == 0) {
            // No expression
            VarID = VarUIDGenerator.GetUID();
            (VarNameToVarIDMapStack.back())[VarName] = VarID;
            (VarIDToVarNameMapStack.back())[VarID] = VarName;
            auto Retval = new ExpressionBase(this, VarID);
            ScopeStack.back()->insert(Retval);
            return Retval;
        } else {
            // There must be a cached expression
            ExpressionBase Temp(this, VarID);
            ExpressionBase* Cached = ScopedLookup(&Temp);
            if (Cached == NULL) {
                throw InternalError((string)"Internal Error: Expected a cached expression");
            }
            return Cached;
        }
    }

    const ExpressionBase* ExpressionManager::CreateAnd(const ExpressionBase* Arg1,
                                                       const ExpressionBase* Arg2)
    {
        vector<const ExpressionBase*> Args;
        Args.push_back(Arg1);
        Args.push_back(Arg2);
        return CreateAnd(Args);
    }

    const ExpressionBase* ExpressionManager::CreateOr(const ExpressionBase* Arg1,
                                                      const ExpressionBase* Arg2)
    {
        vector<const ExpressionBase*> Args;
        Args.push_back(Arg1);
        Args.push_back(Arg2);
        return CreateOr(Args);
    }

    const ExpressionBase* ExpressionManager::CreateXor(const ExpressionBase* Arg1,
                                                       const ExpressionBase* Arg2)
    {
        CheckMgr(Arg1);
        CheckMgr(Arg2);

        if(Arg1 == TrueExp) {
            return CreateNot(Arg2);
        } else if(Arg1 == FalseExp) {
            return Arg2;
        } else if(Arg2 == TrueExp) {
            return CreateNot(Arg1);
        } else if(Arg2 == FalseExp) {
            return Arg1;
        }
        return CreateOp(EXP_XOR, Arg1, Arg2);
    }

    const ExpressionBase* ExpressionManager::CreateXnor(const ExpressionBase* Arg1,
                                                        const ExpressionBase* Arg2)
    {
        CheckMgr(Arg1);
        CheckMgr(Arg2);
        if(Arg1 == TrueExp) {
            return Arg2;
        } else if(Arg1 == FalseExp) {
            return CreateNot(Arg2);
        } else if(Arg2 == TrueExp) {
            return Arg1;
        } else if(Arg2 == FalseExp) {
            return CreateNot(Arg1);
        }
        return CreateOp(EXP_XNOR, Arg1, Arg2);
    }

    const ExpressionBase* ExpressionManager::CreateNand(const ExpressionBase* Arg1,
                                                        const ExpressionBase* Arg2)
    {
        vector<const ExpressionBase*> Args;
        Args.push_back(Arg1);
        Args.push_back(Arg2);
        return CreateNand(Args);
    }

    const ExpressionBase* ExpressionManager::CreateNor(const ExpressionBase* Arg1,
                                                       const ExpressionBase* Arg2)
    {
        vector<const ExpressionBase*> Args;
        Args.push_back(Arg1);
        Args.push_back(Arg2);
        return CreateNor(Args);        
    }

    const ExpressionBase* ExpressionManager::CreateImplies(const ExpressionBase* Arg1,
                                                           const ExpressionBase* Arg2)
    {
        CheckMgr(Arg1);
        CheckMgr(Arg2);
        
        return CreateOp(EXP_IMPLIES, Arg1, Arg2);
    }

    const ExpressionBase* ExpressionManager::CreateIff(const ExpressionBase* Arg1,
                                                       const ExpressionBase* Arg2)
    {
        CheckMgr(Arg1);
        CheckMgr(Arg2);
        return CreateOp(EXP_IFF, Arg1, Arg2);
    }

    const ExpressionBase* ExpressionManager::Migrate(const ExpressionBase* Exp)
    {
        if(Exp->GetMgr() == this) {
            return Exp;
        }

        // Migrate this expression
        switch(Exp->GetOp()) {
        case EXP_VAR:
            return CreateVar(Exp->GetVarName());
        case EXP_TRUE:
            return CreateTrue();
        case EXP_FALSE:
            return CreateFalse();
        default:
            const vector<const ExpressionBase*>& Children = Exp->GetChildren();
            const u32 NumChildren = Children.size();
            vector<const ExpressionBase*> MigChildren;
            for(u32 i = 0; i < NumChildren; ++i) {
                MigChildren.push_back(Migrate(Children[i]));
            }
            return CreateOp(Exp->GetOp(), MigChildren);
        }
    }

    const string& ExpressionManager::GetVarName(u64 VarID) const
    {
        return ScopedLookup(VarID);
    }

} /* End namespace */

