#if !defined __SATLIB_EXPRESSION_MANAGER_HPP
#define __SATLIB_EXPRESSION_MANAGER_HPP

#include "SatLibForwardDecls.hpp"
#include "UIDGenerator.hpp"

namespace SatLib {

    class ExpressionManager
    {
    private:
        static const string EmptyString;
        vector<ExpHashSet*> ScopeStack;
        vector<unordered_map<u64, string> > VarIDToVarNameMapStack;
        vector<unordered_map<string, u64> > VarNameToVarIDMapStack;
        UIDGenerator VarUIDGenerator;

        // The constant true and false expressions
        const ExpressionBase* TrueExp;
        const ExpressionBase* FalseExp;

        // Some utility functions
        inline void ClearScope(ExpHashSet* Scope) const;
        inline ExpressionBase* ScopedLookup(ExpressionBase* Exp) const;
        inline u64 ScopedLookup(const string& VarName) const;
        inline const string& ScopedLookup(u64 VarID) const;
        inline void CheckMgr(const ExpressionBase* Exp) const;
        inline void CheckMgr(const vector<const ExpressionBase*>& Exps) const;

        inline bool CheckTrue(const vector<const ExpressionBase*>& Exps) const;
        inline bool CheckFalse(const vector<const ExpressionBase*>& Exps) const;
        inline vector<const ExpressionBase*> 
        EliminateTrue(const vector<const ExpressionBase*>& Exps) const;
        inline vector<const ExpressionBase*> 
        EliminateFalse(const vector<const ExpressionBase*>& Exps) const;
        
    public:
        ExpressionManager();
        virtual ~ExpressionManager();
        void Push();
        void Pop(u32 NumScopes = 1);

        // Util functions for cloning, etc
        const ExpressionBase* CreateOp(ExpOperator Op, 
                                       const vector<const ExpressionBase*>& Args);
        const ExpressionBase* CreateOp(ExpOperator Op,
                                       const ExpressionBase* Arg1,
                                       const ExpressionBase* Arg2);

        // Constant Expression creation methods
        const ExpressionBase* CreateTrue();
        const ExpressionBase* CreateFalse();

        const ExpressionBase* CreateAnd(const vector<const ExpressionBase*>& Args);
        const ExpressionBase* CreateOr(const vector<const ExpressionBase*>& Args);
        const ExpressionBase* CreateNot(const ExpressionBase* Arg);
        const ExpressionBase* CreateNand(const vector<const ExpressionBase*>& Args);
        const ExpressionBase* CreateNor(const vector<const ExpressionBase*>& Args);
        const ExpressionBase* CreateVar(const string& VarName);

        // Binary expression create routines
        const ExpressionBase* CreateAnd(const ExpressionBase* Arg1, 
                                        const ExpressionBase* Arg2);
        const ExpressionBase* CreateOr(const ExpressionBase* Arg1, 
                                       const ExpressionBase* Arg2);
        const ExpressionBase* CreateXor(const ExpressionBase* Arg1, 
                                        const ExpressionBase* Arg2);
        const ExpressionBase* CreateXnor(const ExpressionBase* Arg1, 
                                         const ExpressionBase* Arg2);
        const ExpressionBase* CreateNand(const ExpressionBase* Arg1, 
                                         const ExpressionBase* Arg2);
        const ExpressionBase* CreateNor(const ExpressionBase* Arg1, 
                                        const ExpressionBase* Arg2);
        const ExpressionBase* CreateImplies(const ExpressionBase* Ant, 
                                            const ExpressionBase* Con);
        const ExpressionBase* CreateIff(const ExpressionBase* Arg1, 
                                        const ExpressionBase* Arg2);

        // Migrate and expression from another expression manager to this one
        const ExpressionBase* Migrate(const ExpressionBase* Exp);

        const string& GetVarName(u64 VarID) const;
    };

} /* End namespace */

#endif /* __SATLIB_EXPRESSION_MANAGER_HPP */

