#include <SatLibZ3Solver.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <SatLibExceptions.hpp>
#include <ExpressionBase.hpp>

namespace SatLib {

    SatLibZ3Solver::SatLibZ3Solver()
        : PluginLoaderBase("libz3.so"),
          SatLibSolverBase("Z3Solver"),
          Z3_mk_config((Z3Config* (*)())ResolveSymbol("Z3_mk_config")),
          Z3_del_config((void (*)(Z3Config*))ResolveSymbol("Z3_del_config")),
          Z3_set_param_value((void (*)(Z3Config*, Z3String, Z3String))ResolveSymbol("Z3_set_param_value")),
          Z3_mk_context_rc((Z3Ctx* (*)(Z3Config*))ResolveSymbol("Z3_mk_context_rc")),
          Z3_del_context((void (*)(Z3Ctx*))ResolveSymbol("Z3_del_context")),
          Z3_mk_solver((Z3Solver* (*)(Z3Ctx*))ResolveSymbol("Z3_mk_solver")),
          Z3_mk_string_symbol((Z3Symbol* (*)(Z3Ctx*, Z3String))ResolveSymbol("Z3_mk_string_symbol")),
          Z3_mk_int_symbol((Z3Symbol* (*)(Z3Ctx*, i32))ResolveSymbol("Z3_mk_int_symbol")),
          Z3_mk_bool_sort((Z3Sort* (*)(Z3Ctx*))ResolveSymbol("Z3_mk_bool_sort")),
          Z3_inc_ref((void (*)(Z3Ctx*, Z3Ast*))ResolveSymbol("Z3_inc_ref")),
          Z3_dec_ref((void (*)(Z3Ctx*, Z3Ast*))ResolveSymbol("Z3_dec_ref")),
          Z3_solver_inc_ref((void (*)(Z3Ctx*, Z3Solver*))ResolveSymbol("Z3_solver_inc_ref")),
          Z3_solver_dec_ref((void (*)(Z3Ctx*, Z3Solver*))ResolveSymbol("Z3_solver_dec_ref")),
          Z3_model_inc_ref((void (*)(Z3Ctx*, Z3Model*))ResolveSymbol("Z3_model_inc_ref")),
          Z3_model_dec_ref((void (*)(Z3Ctx*, Z3Model*))ResolveSymbol("Z3_model_dec_ref")),        
          Z3_mk_const((Z3Ast* (*)(Z3Ctx*, Z3Symbol*, Z3Sort*))ResolveSymbol("Z3_mk_const")),
          Z3_mk_true((Z3Ast* (*)(Z3Ctx*))ResolveSymbol("Z3_mk_true")),
          Z3_mk_false((Z3Ast* (*)(Z3Ctx*))ResolveSymbol("Z3_mk_false")),
          Z3_mk_and((Z3Ast* (*)(Z3Ctx*, i32, Z3Ast**))ResolveSymbol("Z3_mk_and")),
          Z3_mk_or((Z3Ast* (*)(Z3Ctx*, i32, Z3Ast**))ResolveSymbol("Z3_mk_or")),
          Z3_mk_not((Z3Ast* (*)(Z3Ctx*, Z3Ast*))ResolveSymbol("Z3_mk_not")),
          Z3_mk_iff((Z3Ast* (*)(Z3Ctx*, Z3Ast*, Z3Ast*))ResolveSymbol("Z3_mk_iff")),
          Z3_mk_implies((Z3Ast* (*)(Z3Ctx*, Z3Ast*, Z3Ast*))ResolveSymbol("Z3_mk_implies")),
          Z3_mk_xor((Z3Ast* (*)(Z3Ctx*, Z3Ast*, Z3Ast*))ResolveSymbol("Z3_mk_xor")),
          Z3_solver_get_model((Z3Model* (*)(Z3Ctx*, Z3Solver*))ResolveSymbol("Z3_solver_get_model")),
          Z3_model_get_num_consts((u32 (*)(Z3Ctx*, Z3Model*))ResolveSymbol("Z3_model_get_num_consts")),
          Z3_model_get_const_decl((Z3FuncDecl* (*)(Z3Ctx*, Z3Model*, u32))ResolveSymbol("Z3_model_get_const_decl")),
          Z3_model_get_const_interp((Z3Ast* (*)(Z3Ctx*, Z3Model*, Z3FuncDecl*))
                                    ResolveSymbol("Z3_model_get_const_interp")),
          Z3_get_decl_name((Z3Symbol* (*)(Z3Ctx*, Z3FuncDecl*))ResolveSymbol("Z3_get_decl_name")),
          Z3_get_symbol_string((Z3String (*)(Z3Ctx*, Z3Symbol*))ResolveSymbol("Z3_get_symbol_string")),
          Z3_solver_check((Z3SolveStatus (*)(Z3Ctx*, Z3Solver*))ResolveSymbol("Z3_solver_check")),
          Z3_solver_assert((void (*)(Z3Ctx*, Z3Solver*, Z3Ast*))ResolveSymbol("Z3_solver_assert")),
          Z3_ast_to_string((Z3String (*)(Z3Ctx*, Z3Ast*))ResolveSymbol("Z3_ast_to_string")),
          Z3_solver_push((void (*)(Z3Ctx*, Z3Solver*))ResolveSymbol("Z3_solver_push")),
          Z3_solver_pop((void (*)(Z3Ctx*, Z3Solver*, u32))ResolveSymbol("Z3_solver_pop")),
          Z3_solver_get_num_scopes((u32 (*)(Z3Ctx*, Z3Solver*))ResolveSymbol("Z3_solver_get_num_scopes"))
    {
        // Create the context and the solver objects
        Z3Config* Cfg = Z3_mk_config();
        Z3_set_param_value(Cfg, "MODEL_COMPLETION", "true");
        TheContext = Z3_mk_context_rc(Cfg);
        Z3_del_config(Cfg);
        TheSolver = Z3_mk_solver(TheContext);
        Z3_solver_inc_ref(TheContext, TheSolver);
        BoolSort = Z3_mk_bool_sort(TheContext);
        Z3_inc_ref(TheContext, (Z3Ast*)BoolSort);

        LastSolveResult = SOLVE_UNKNOWN;
    }

    SatLibZ3Solver::~SatLibZ3Solver()
    {
        Z3_solver_dec_ref(TheContext, TheSolver);
        Z3_del_context(TheContext);
    }

    Z3Ast* SatLibZ3Solver::MkVar(const string& VarName)
    {
        auto Sym = Z3_mk_string_symbol(TheContext, VarName.c_str());
        auto Retval = Z3_mk_const(TheContext, Sym, BoolSort);
        IncRef(Retval);
        return Retval;
    }

    Z3Ast* SatLibZ3Solver::MkTrue()
    {
        auto Retval = Z3_mk_true(TheContext);
        IncRef(Retval);
        return Retval;
    }

    Z3Ast* SatLibZ3Solver::MkFalse()
    {
        auto Retval = Z3_mk_false(TheContext);
        IncRef(Retval);
        return Retval;
    }

    Z3Ast* SatLibZ3Solver::MkAnd(const vector<Z3Ast*>& Args)
    {
        const u32 NumArgs = Args.size();
        auto ArgArray = (Z3Ast**)malloc(sizeof(Z3Ast*) * NumArgs);
        for (u32 i = 0; i < NumArgs; ++i) {
            ArgArray[i] = Args[i];
        }
        
        auto Retval = Z3_mk_and(TheContext, NumArgs, ArgArray);
        IncRef(Retval);
        free(ArgArray);
        return Retval;
    }

    Z3Ast* SatLibZ3Solver::MkOr(const vector<Z3Ast*>& Args)
    {
        const u32 NumArgs = Args.size();
        auto ArgArray = (Z3Ast**)malloc(sizeof(Z3Ast*) * NumArgs);
        for (u32 i = 0; i < NumArgs; ++i) {
            ArgArray[i] = Args[i];
        }
        
        auto Retval = Z3_mk_or(TheContext, NumArgs, ArgArray);
        IncRef(Retval);
        free(ArgArray);
        return Retval;
    }

    Z3Ast* SatLibZ3Solver::MkNot(Z3Ast* Arg)
    {
        auto Retval = Z3_mk_not(TheContext, Arg);
        IncRef(Retval);
        return Retval;
    }

    Z3Ast* SatLibZ3Solver::MkIff(Z3Ast* Arg1, Z3Ast* Arg2)
    {
        auto Retval = Z3_mk_iff(TheContext, Arg1, Arg2);
        IncRef(Retval);
        return Retval;
    }

    Z3Ast* SatLibZ3Solver::MkImplies(Z3Ast* Arg1, Z3Ast* Arg2)
    {
        auto Retval = Z3_mk_implies(TheContext, Arg1, Arg2);
        IncRef(Retval);
        return Retval;
    }
   
    Z3Ast* SatLibZ3Solver::MkXor(Z3Ast* Arg1, Z3Ast* Arg2)
    {
        auto Retval = Z3_mk_xor(TheContext, Arg1, Arg2);
        IncRef(Retval);
        return Retval;
    }

    void SatLibZ3Solver::IncRef(Z3Ast* Ast)
    {
        Z3_inc_ref(TheContext, Ast);
    }

    void SatLibZ3Solver::DecRef(Z3Ast* Ast)
    {
        Z3_dec_ref(TheContext, Ast);
    }

    void SatLibZ3Solver::Push()
    {
        SatLibSolverBase::Push();
        Z3_solver_push(TheContext, TheSolver);
    }

    void SatLibZ3Solver::Pop(u32 NumScopes)
    {
        SatLibSolverBase::Pop(NumScopes);
        Z3_solver_pop(TheContext, TheSolver, NumScopes);
    }

    SolveResult SatLibZ3Solver::Solve()
    {
        // just pass down to the Z3 solver
        auto Z3Stat = Z3_solver_check(TheContext, TheSolver);
        if (Z3Stat == Z3_L_FALSE) {
            LastSolveResult = SOLVE_UNSAT;
            return SOLVE_UNSAT;
        } else if(Z3Stat == Z3_L_TRUE) {
            LastSolveResult = SOLVE_SAT;
            return SOLVE_SAT;
        } else {
            LastSolveResult = SOLVE_UNKNOWN;
            return SOLVE_UNKNOWN;
        }
    }

    void SatLibZ3Solver::Assert(PropFormula Prop)
    {
        // Call the subclass method as well
        SatLibSolverBase::Assert(Prop);
        
        auto Z3Prop = ExpToZ3::Do(Prop, this);
        Z3_solver_assert(TheContext, TheSolver, Z3Prop);
        DecRef(Z3Prop);
    }

    void SatLibZ3Solver::GetModel(SatModel& Model)
    {
        if (LastSolveResult != SOLVE_SAT) {
            throw SolverException("Called GetModel, but Last solve was not a sat!");            
        }

        Model.clear();
        // This is a little complex, we need to enumerate over all constants
        // construct an apply, evaluate the constant in the model
        // stringify the result and compare it to true or false
        auto TheZ3Model = Z3_solver_get_model(TheContext, TheSolver);
        Z3_model_inc_ref(TheContext, TheZ3Model);

        const u32 NumConsts = Z3_model_get_num_consts(TheContext, TheZ3Model);
        for (u32 i = 0; i < NumConsts; ++i) {
            Z3FuncDecl* CurDecl = Z3_model_get_const_decl(TheContext, TheZ3Model, i);

            // Get the name of the variable
            Z3Symbol* DeclNameSym = Z3_get_decl_name(TheContext, CurDecl);
            string VarName = string(Z3_get_symbol_string(TheContext, DeclNameSym));

            // Get the value of the variable
            Z3_inc_ref(TheContext, (Z3Ast*)CurDecl);
            Z3Ast* CurInterp = Z3_model_get_const_interp(TheContext, TheZ3Model, CurDecl);

            // Doesn't matter? Just set it to true
            if (CurInterp == NULL) {
                Z3_dec_ref(TheContext, (Z3Ast*)CurDecl);
                Model[VarName] = VAR_TRUE;
                continue;
            }

            Z3_inc_ref(TheContext, CurInterp);
            auto ValStr = string(Z3_ast_to_string(TheContext, CurInterp));
            Z3_dec_ref(TheContext, CurInterp);
            Z3_dec_ref(TheContext, (Z3Ast*)CurDecl);
            boost::algorithm::trim(ValStr);
            boost::algorithm::to_lower(ValStr);
            
            if (ValStr == "true") {
                Model[VarName] = VAR_TRUE;
            } else if (ValStr == "false") {
                Model[VarName] = VAR_FALSE;
            } else {
                Model[VarName] = VAR_UNDEF;
            }
        }

        Z3_model_dec_ref(TheContext, TheZ3Model);
        return;
    }

    // ExpToZ3 implementation
    ExpToZ3::ExpToZ3(SatLibZ3Solver* Z3Solver)
        : ExpressionVisitorBase("ExpToZ3"),
          TheSolver(Z3Solver)
    {
        // Nothing here
    }

    ExpToZ3::~ExpToZ3()
    {
        // Nothing here
    }

    void ExpToZ3::PreVisit(const ExpressionBase* Exp) 
    {
        // Nothing here
    }

    void ExpToZ3::Visit(const ExpressionBase* Exp, u32 VisitNum) 
    {
        // Nothing here
    }

    void ExpToZ3::PostVisit(const ExpressionBase* Exp)
    {
        switch(Exp->GetOp()) {
        case EXP_AND:
        case EXP_OR:
        case EXP_NAND:
        case EXP_NOR:
            {
                const u32 NumChildren = Exp->GetChildren().size();
                vector<Z3Ast*> Conjuncts(NumChildren);
                for (u32 i = 0; i < NumChildren; ++i) {
                    Conjuncts[NumChildren - i - 1] = ExpStack.back();
                    ExpStack.pop_back();
                }

                switch(Exp->GetOp()) {
                case EXP_AND: {
                    ExpStack.push_back(TheSolver->MkAnd(Conjuncts));
                    break;
                }
                case EXP_OR: {
                    ExpStack.push_back(TheSolver->MkOr(Conjuncts));
                    break;
                }
                case EXP_NAND: {
                    auto AndExp = TheSolver->MkAnd(Conjuncts);
                    ExpStack.push_back(TheSolver->MkNot(AndExp));
                    TheSolver->DecRef(AndExp);
                    break;
                }
                case EXP_NOR: {
                    auto OrExp = TheSolver->MkOr(Conjuncts);
                    ExpStack.push_back(TheSolver->MkNot(OrExp));
                    TheSolver->DecRef(OrExp);
                    break;
                }
                default:
                    assert(false);
                }
                // Decrement refs of all children
                for (auto Child : Conjuncts) {
                    TheSolver->DecRef(Child);
                }
                break;
            }

        case EXP_XOR:
        case EXP_XNOR:
        case EXP_IMPLIES:
        case EXP_IFF:
            {
                auto Arg1 = ExpStack.back();
                ExpStack.pop_back();
                auto Arg2 = ExpStack.back();
                ExpStack.pop_back();

                switch(Exp->GetOp()) {
                case EXP_XNOR:
                case EXP_XOR: {
                    auto XorExp = TheSolver->MkXor(Arg1, Arg2);
                    if (Exp->GetOp() == EXP_XNOR) {
                        ExpStack.push_back(TheSolver->MkNot(XorExp));
                        TheSolver->DecRef(XorExp);
                    } else {
                        ExpStack.push_back(XorExp);
                    }
                    break;
                }
                case EXP_IMPLIES:
                    ExpStack.push_back(TheSolver->MkImplies(Arg2, Arg1));
                    break;

                case EXP_IFF:
                    ExpStack.push_back(TheSolver->MkIff(Arg1, Arg2));
                    break;
                   
                default:
                    assert(false);
                }
                // dec ref the children
                TheSolver->DecRef(Arg1);
                TheSolver->DecRef(Arg2);
                break;
            }

        case EXP_VAR:
            ExpStack.push_back(TheSolver->MkVar(Exp->GetVarName()));
            break;

        case EXP_TRUE:
            ExpStack.push_back(TheSolver->MkTrue());
            break;
            
        case EXP_FALSE:
            ExpStack.push_back(TheSolver->MkFalse());
            break;
        }
    }

    Z3Ast* ExpToZ3::Do(const ExpressionBase* Exp, SatLibZ3Solver* TheSolver)
    {
        ExpToZ3 Convertor(TheSolver);
        Exp->Accept(&Convertor);
        if (Convertor.ExpStack.size() != 1) {
            throw InternalError((string)"Internal Error: Expected size of ExpStack to be " + 
                                "one after ExpToZ3");
        }
        auto Retval = Convertor.ExpStack.back();
        Convertor.ExpStack.pop_back();
        return Retval;
    }

} /* end namespace */

