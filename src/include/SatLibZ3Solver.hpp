#if !defined __SATLIB_Z3_SOLVER_HPP
#define __SATLIB_Z3_SOLVER_HPP

#include "SatLibForwardDecls.hpp"
#include "SatLibSolverBase.hpp"
#include "PluginLoaderBase.hpp"
#include "ExpressionVisitorBase.hpp"

namespace SatLib {

    struct Z3Ctx;
    struct Z3Solver;
    struct Z3Ast;
    struct Z3Sort;
    struct Z3Symbol;
    struct Z3Model;
    struct Z3FuncDecl;
    typedef const char* Z3String;
    struct Z3Config;
    
    enum Z3SolveStatus {
         Z3_L_FALSE = -1,
         Z3_L_UNDEF,
         Z3_L_TRUE
    };

    class SatLibZ3Solver : public PluginLoaderBase, public SatLibSolverBase
    {
    private:
        Z3Config* (*Z3_mk_config)();
        void (*Z3_del_config)(Z3Config*);
        void (*Z3_set_param_value)(Z3Config*, Z3String, Z3String);

        Z3Ctx* (*Z3_mk_context_rc)(Z3Config* Cfg);
        void (*Z3_del_context)(Z3Ctx*);
        Z3Solver* (*Z3_mk_solver)(Z3Ctx*);

        Z3Symbol* (*Z3_mk_string_symbol)(Z3Ctx*, Z3String);
        Z3Symbol* (*Z3_mk_int_symbol)(Z3Ctx*, i32);
        Z3Sort* (*Z3_mk_bool_sort)(Z3Ctx*);

        void (*Z3_inc_ref)(Z3Ctx*, Z3Ast*);
        void (*Z3_dec_ref)(Z3Ctx*, Z3Ast*);
        void (*Z3_solver_inc_ref)(Z3Ctx*, Z3Solver*);
        void (*Z3_solver_dec_ref)(Z3Ctx*, Z3Solver*);
        void (*Z3_model_inc_ref)(Z3Ctx*, Z3Model*);
        void (*Z3_model_dec_ref)(Z3Ctx*, Z3Model*);

        // Expression creation
        Z3Ast* (*Z3_mk_const)(Z3Ctx*, Z3Symbol*, Z3Sort*);
        Z3Ast* (*Z3_mk_true)(Z3Ctx*);
        Z3Ast* (*Z3_mk_false)(Z3Ctx*);
        Z3Ast* (*Z3_mk_and)(Z3Ctx*, i32, Z3Ast**);
        Z3Ast* (*Z3_mk_or)(Z3Ctx*, i32, Z3Ast**);
        Z3Ast* (*Z3_mk_not)(Z3Ctx*, Z3Ast*);
        Z3Ast* (*Z3_mk_iff)(Z3Ctx*, Z3Ast*, Z3Ast*);
        Z3Ast* (*Z3_mk_implies)(Z3Ctx*, Z3Ast*, Z3Ast*);
        Z3Ast* (*Z3_mk_xor)(Z3Ctx*, Z3Ast*, Z3Ast*);

        // Models and funcs for evaluating models
        Z3Model* (*Z3_solver_get_model)(Z3Ctx*, Z3Solver*);
        u32 (*Z3_model_get_num_consts)(Z3Ctx*, Z3Model*);
        Z3FuncDecl* (*Z3_model_get_const_decl)(Z3Ctx*, Z3Model*, u32);
        Z3Ast* (*Z3_model_get_const_interp)(Z3Ctx*, Z3Model*, Z3FuncDecl*);
        Z3Symbol* (*Z3_get_decl_name)(Z3Ctx*, Z3FuncDecl*);
        Z3String (*Z3_get_symbol_string)(Z3Ctx*, Z3Symbol*);

        // Checking
        Z3SolveStatus (*Z3_solver_check)(Z3Ctx*, Z3Solver*);

        // Assertions
        void (*Z3_solver_assert)(Z3Ctx*, Z3Solver*, Z3Ast*);

        // Stringification
        Z3String (*Z3_ast_to_string)(Z3Ctx*, Z3Ast*);

        // Scoping
        void (*Z3_solver_push)(Z3Ctx*, Z3Solver*);
        void (*Z3_solver_pop)(Z3Ctx*, Z3Solver*, u32);
        u32 (*Z3_solver_get_num_scopes)(Z3Ctx*, Z3Solver*);

        // Internal handles
        Z3Ctx* TheContext;
        Z3Solver* TheSolver;
        Z3Sort* BoolSort;

        // Typestate vars :-(
        SolveResult LastSolveResult;

    public:
        SatLibZ3Solver();
        virtual ~SatLibZ3Solver();

        // Interface to Z3 methods
        Z3Ast* MkVar(const string& VarName);
        Z3Ast* MkTrue();
        Z3Ast* MkFalse();
        Z3Ast* MkAnd(const vector<Z3Ast*>& Args);
        Z3Ast* MkNot(Z3Ast* Arg);
        Z3Ast* MkOr(const vector<Z3Ast*>& Args);
        Z3Ast* MkXor(Z3Ast* Arg1, Z3Ast* Arg2);
        Z3Ast* MkIff(Z3Ast* Arg1, Z3Ast* Arg2);
        Z3Ast* MkImplies(Z3Ast* Arg1, Z3Ast* Arg2);
        void IncRef(Z3Ast* Ast);
        void DecRef(Z3Ast* Ast);

        virtual void Push() override;
        virtual void Pop(u32 NumScopes = 1) override;
        virtual SolveResult Solve() override;
        virtual void GetModel(SatModel& Model) override;
        virtual void Assert(PropFormula Prop) override;
    };

    // A convertor for converting to Z3 expressions
    class ExpToZ3 : public ExpressionVisitorBase
    {
    private:
        vector<Z3Ast*> ExpStack;
        SatLibZ3Solver* TheSolver;
        
    public:
        ExpToZ3(SatLibZ3Solver* Z3Solver);
        virtual ~ExpToZ3();
        
        virtual void PreVisit(const ExpressionBase* Exp) override;
        virtual void Visit(const ExpressionBase* Exp, u32 VisitNum) override;
        virtual void PostVisit(const ExpressionBase* Exp) override;

        static Z3Ast* Do(const ExpressionBase* Exp, SatLibZ3Solver* TheSolver);
    };

} /* end namespace */

#endif /* __SATLIB_Z3_SOLVER_HPP */


