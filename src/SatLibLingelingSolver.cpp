#include <SatLibLingelingSolver.hpp>
#include <SatLibForwardDecls.hpp>
#include <SatLibSolverBase.hpp>
#include <unordered_map>
#include <SimpleNNFConverter.hpp>
#include <TseitinTransform.hpp>
#include <TseitinTransformFull.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <Literal.hpp>
#include <ExpressionManager.hpp>
#include <SatLibExceptions.hpp>
#include <ExpressionBase.hpp>
#include <Utils.hpp>


namespace SatLib {

    SatLibLingelingSolver::SatLibLingelingSolver(const string& SolverPluginName)
        : SatLibSolverBase((string)"LingelingSolver-" + SolverPluginName),
          TheSolver(new LingelingLoader(SolverPluginName)),
          LastSolveNumAssertions(0),
          LastSolveResult(SOLVE_UNKNOWN)
    {
        // Nothing here
    }

    SatLibLingelingSolver::~SatLibLingelingSolver()
    {
        delete TheSolver;
        VMap.clear();
    }


    SolveResult SatLibLingelingSolver::Solve()
    {
        // Pick up all the assertions and turn them into clauses
        // NNFConverterBase* NNFConverter = new SimpleNNFConverter();
        CNFConverterBase* CNFConverter = new TseitinTransformFull();
        
        const u32 NumAssertions = Assertions.size();
        for(u32 j = LastSolveNumAssertions; j < NumAssertions; ++j) {
            // Mgr->Push();
            PropFormula Prop = Assertions[j];
            // cout << "Converting " << *Prop << " to NNF..." << endl;
            // cout.flush();
            // Prop = (*NNFConverter)(Mgr, Prop);
            // cout << "NNF: " << *NNFProp << endl;
            // cout.flush();
            vector<Clause> ClauseVec;
            (*CNFConverter)(Prop, ClauseVec);
                
            // Assertions are now in the clause vec
            // Push them onto the solver and create the mappings
            const u32 NumClauses = ClauseVec.size();
            for(u32 k = 0; k < NumClauses; ++k) {
                const u32 NumLits = ClauseVec[k].size();
                vector<i32> LitVec(NumLits);
                for(u32 l = 0; l < NumLits; ++l) {
                    unordered_map<u32, i32>::const_iterator Finder;
                    auto const& CurLit = ClauseVec[k][l];
                    if ((Finder = VMap.find(CurLit.GetVarID())) == VMap.end()) {
                        i32 LitVar = TheSolver->IncVar();
                        VMap[CurLit.GetVarID()] = LitVar;
                        LitVec[l] = (CurLit.IsNegated() ? -LitVar : LitVar);
                    } else {
                        i32 LitVar = Finder->second;
                        LitVec[l] = (CurLit.IsNegated() ? -LitVar : LitVar);
                    }
                }
                
                // Add this clause to the solver
                for(auto const& Lit : LitVec) {
                    TheSolver->Add(Lit);
                }
                TheSolver->Add(0);
            }
            // cout << "Prop: " << *Prop << ", NNF: " << *NNFProp << endl;
            // Mgr->Pop();
        }

        // Clear up all the temporary context created
        // delete NNFConverter;
        delete CNFConverter;

        // All assertions are pushed onto the solver
        // Solve!
        if(TheSolver->Sat()) {
            LastSolveResult = SOLVE_SAT;
        } else {
            LastSolveResult = SOLVE_UNSAT;
        }
        LastSolveNumAssertions = Assertions.size();
        return LastSolveResult;

    }

    void SatLibLingelingSolver::GetModel(SatModel& Model)
    {
        if(LastSolveResult != SOLVE_SAT) {
            throw SolverException("Called GetModel, but Last solve was not a sat!");
        }
        
        Model.clear();
        for(auto it = VMap.begin(); it != VMap.end(); ++it) {
            if(it->first >= SATLIB_AUX_VAR_OFFSET) {
                continue;
            }
            auto VarVal = TheSolver->Deref(it->second);
            auto const& VarName = Mgr->GetVarName(it->first);
            
            if (VarName == "") {
                throw InternalError((string)"Internal Error: Expected variable name to be " +
                                    "non-empty :-(");
            }
            
            if(VarVal > 0) {
                Model[VarName] = VAR_TRUE;
            } else if (VarVal < 0) {
                Model[VarName] = VAR_FALSE;
            } else {
                Model[VarName] = VAR_UNDEF;
            }
        }
        return;

    }

    void SatLibLingelingSolver::Push()
    {
        SatLibSolverBase::Push();
    }

    void SatLibLingelingSolver::Pop(u32 NumScopes)
    {
        SatLibSolverBase::Pop(NumScopes);
        TheSolver->Reset();
        LastSolveNumAssertions = 0;
    }

} /* end namespace */

