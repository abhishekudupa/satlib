#if !defined __SATLIB_SATLIB_MINISAT_BASED_SOLVER_HPP
#define __SATLIB_SATLIB_MINISAT_BASED_SOLVER_HPP

#include "SatLibForwardDecls.hpp"
#include "SatLibSolverBase.hpp"
#include <unordered_map>
#include "SimpleNNFConverter.hpp"
#include "TseitinTransform.hpp"
#include <boost/algorithm/string/predicate.hpp>
#include "Literal.hpp"
#include "ExpressionManager.hpp"
#include "SatLibExceptions.hpp"
#include "ExpressionBase.hpp"
#include "Utils.hpp"
#include "TseitinTransformFull.hpp"

namespace SatLib {

    // Expects the template argument to have typedefs for:
    // 1. VarType
    // 2. SolverType
    // 3. VecType
    // 4. LitType
    // Expects functions:
    // 1. mkLit
    // Also expects values
    // 1. l_False
    // 2. l_True
    // 3. l_Undef
    // 4. SolverName
    
    template<typename T>
    class SatLibMinisatBasedSolver : public SatLibSolverBase
    {
    private:
        typedef typename T::VariableMapType VariableMapType;
        typedef typename T::SolverType SolverType;
        VariableMapType VMap;
        SolverType* TheSolver;
        i32 LastSolveNumAssertions;
        SolveResult LastSolveResult;

    public:
        SatLibMinisatBasedSolver();
        virtual ~SatLibMinisatBasedSolver();

        virtual SolveResult Solve() override;
        virtual void GetModel(SatModel& Model) override;

        virtual void Push() override;
        virtual void Pop(u32 NumScopes = 1) override;
    };

    // Implementation
    template<typename T>
    SatLibMinisatBasedSolver<T>::SatLibMinisatBasedSolver()
        : SatLibSolverBase(T::SolverName), 
          LastSolveNumAssertions(0)
    {
        TheSolver = new SolverType();
    }

    template<typename T>
    SatLibMinisatBasedSolver<T>::~SatLibMinisatBasedSolver()
    {
        delete TheSolver;
        VMap.clear();
    }

    template<typename T>
    SolveResult SatLibMinisatBasedSolver<T>::Solve()
    {
        // Pick up all the assertions and turn them into clauses
        // NNFConverterBase* NNFConverter = new SimpleNNFConverter();
        // CNFConverterBase* CNFConverter = new TseitinTransform();
        CNFConverterBase* CNFConverter = new TseitinTransformFull();

        // cout << "Solver: Beginning Solve, CNF conversion..." << endl;

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
            ClauseVec.reserve(128);
            (*CNFConverter)(Prop, ClauseVec);
                
            // Assertions are now in the clause vec
            // Push them onto the solver and create the mappings
            const u32 NumClauses = ClauseVec.size();
            for(u32 k = 0; k < NumClauses; ++k) {
                const u32 NumLits = ClauseVec[k].size();
                typename T::template VecType<typename T::LitType> LitVec(NumLits);
                for(u32 l = 0; l < NumLits; ++l) {
                    typename T::VariableMapType::const_iterator Finder;
                    auto const& CurLit = ClauseVec[k][l];
                    if ((Finder = VMap.find(CurLit.GetVarID())) == VMap.end()) {
                        typename T::VarType LitVar = TheSolver->newVar();
                        VMap[CurLit.GetVarID()] = LitVar;
                        LitVec[l] = T::mkLit(LitVar, CurLit.IsNegated());
                    } else {
                        LitVec[l] = T::mkLit(Finder->second, CurLit.IsNegated());
                    }
                }
                TheSolver->addClause(LitVec);
            }
                
            // cout << "Prop: " << *Prop << ", NNF: " << *NNFProp << endl;
            // Mgr->Pop();
        }

        // cout << "Solver: CNF conversion done, starting SAT solve..." << endl;

        // Clear up all the temporary context created
        // delete NNFConverter;
        delete CNFConverter;

        // All assertions are pushed onto the solver
        // Solve!
        if(TheSolver->solve()) {
            LastSolveResult = SOLVE_SAT;
        } else {
            LastSolveResult = SOLVE_UNSAT;
        }

        // cout << "SAT solve done!" << endl;
        LastSolveNumAssertions = Assertions.size();
        return LastSolveResult;
    }

    template<typename T>
    void SatLibMinisatBasedSolver<T>::GetModel(SatModel& Model)
    {
        if(LastSolveResult != SOLVE_SAT) {
            throw SolverException("Called GetModel, but Last solve was not a sat!");
        }
        
        Model.clear();
        for(typename T::VariableMapType::const_iterator it = VMap.begin(); it != VMap.end(); ++it) {
            if(it->first >= SATLIB_AUX_VAR_OFFSET) {
                continue;
            }
            auto VarVal = TheSolver->modelValue(it->second);
            auto const& VarName = Mgr->GetVarName(it->first);
            if (VarName == "") {
                throw InternalError((string)"Internal Error: Did not expect variable name to be " + 
                                    "the empty string :-(");
            }
            if(VarVal == T::l_True) {
                Model[VarName] = VAR_TRUE;
            } else if (VarVal == T::l_False) {
                Model[VarName] = VAR_FALSE;
            } else {
                Model[VarName] = VAR_UNDEF;
            }
        }
        return;
    }

    template<typename T>
    void SatLibMinisatBasedSolver<T>::Push()
    {
        SatLibSolverBase::Push();
    }

    template<typename T>
    void SatLibMinisatBasedSolver<T>::Pop(u32 Numscopes)
    {
        // Minisat based solvers do not support backtracking.
        // So we simply delete and recreate the solver, and 
        // set the LastSolveNumAssertions to 0
        delete TheSolver;
        TheSolver = new SolverType();
        LastSolveNumAssertions = 0;
        
        // Call the pop on the base calss
        SatLibSolverBase::Pop(Numscopes);
    }

} /* End namespace */

#endif /* __SATLIB_MINISAT_BASED_SOLVER_HPP */

