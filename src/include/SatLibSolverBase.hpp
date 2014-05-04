#if !defined __SATLIB_SATLIB_SOLVER_BASE_HPP
#define __SATLIB_SATLIB_SOLVER_BASE_HPP

#include "SatLibForwardDecls.hpp"

namespace SatLib {

    /* Base class for all SAT Solvers */
    /* 
       Provides primitives to create propositional expressions
       and solving them
    */
    class SatLibSolverBase
    {
    protected:
        ExpressionManager* Mgr;
        string SolverName;
        vector<PropFormula> Assertions;
        vector<u32> BacktrackPoints;
        
    public:
        SatLibSolverBase(const string& SolverName);
        virtual ~SatLibSolverBase();

        const string& GetName() const;
        
        // Ctx management
        virtual void Push();
        virtual void Pop(u32 NumScopes = 1);
        u32 GetNumScopes() const;
        
        // Formula creation
        PropFormula CreateAnd(const PropVec& Args);
        PropFormula CreateOr(const PropVec& Args);
        PropFormula CreateNot(PropFormula Arg);
        PropFormula CreateNand(const PropVec& Args);
        PropFormula CreateNor(const PropVec& Args);
        PropFormula CreateVar(const string& VarName);
        
        PropFormula CreateAnd(PropFormula Arg1, PropFormula Arg2);
        PropFormula CreateOr(PropFormula Arg1, PropFormula Arg2);
        PropFormula CreateXor(PropFormula Arg1, PropFormula Arg2);
        PropFormula CreateXnor(PropFormula Arg1, PropFormula Arg2);
        PropFormula CreateNand(PropFormula Arg1, PropFormula Arg2);
        PropFormula CreateNor(PropFormula Arg1, PropFormula Arg2);
        PropFormula CreateImplies(PropFormula Arg1, PropFormula Arg2);
        PropFormula CreateIff(PropFormula Arg1, PropFormula Arg2);
        
        // Constant creation
        PropFormula CreateFalse();
        PropFormula CreateTrue();

        virtual void Assert(PropFormula Prop);
        const vector<PropFormula>& GetAssertions();

        virtual SolveResult Solve() = 0;
        virtual void GetModel(SatModel& Model) = 0;

        static SatLibSolverBase* CreateSolver(SLSolverType Type);
    };

} /* End namespace */

#endif /* __SATLIB_SATLIB_SOLVER_BASE_HPP */

