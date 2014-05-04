#if !defined __SATLIB_SATLIB_MINISAT_SOLVER_HPP
#define __SATLIB_SATLIB_MINISAT_SOLVER_HPP

#include "SatLibMinisatBasedSolver.hpp"
#include "../minisat/simp/SimpSolver.h"
#include <unordered_map>

namespace SatLib {

    struct MinisatTypeStruct
    {
        typedef Minisat::Var VarType;
        typedef Minisat::Solver SolverType;
        template<typename T>
        using VecType = Minisat::vec<T>;
        typedef Minisat::Lit LitType;
        typedef unordered_map<u32, VarType> VariableMapType;
        
        static LitType mkLit(VarType Var, bool Sign);
        static const Minisat::lbool l_True;
        static const Minisat::lbool l_False;
        static const Minisat::lbool l_Undef;
        static const string SolverName;
    };


    typedef SatLibMinisatBasedSolver<MinisatTypeStruct> SatLibMinisatSolver;

} /* end namespace */

#endif /* __SATLIB_SATLIB_MINISAT_SOLVER_HPP */

