#if !defined __SATLIB_SATLIB_GLUCOSE_SOLVER_HPP
#define __SATLIB_SATLIB_GLUCOSE_SOLVER_HPP

#include "SatLibMinisatBasedSolver.hpp"
#include "../glucose/simp/SimpSolver.h"
#include <unordered_map>

namespace SatLib {

    struct GlucoseTypeStruct
    {
        typedef Glucose::Var VarType;
        typedef Glucose::Solver SolverType;
        template<typename T>
        using VecType = Glucose::vec<T>;
        typedef Glucose::Lit LitType;
        typedef unordered_map<u32, VarType> VariableMapType;
        
        static LitType mkLit(VarType Var, bool Sign);
        static const Glucose::lbool l_True;
        static const Glucose::lbool l_False;
        static const Glucose::lbool l_Undef;
        static const string SolverName;
    };

    typedef SatLibMinisatBasedSolver<GlucoseTypeStruct> SatLibGlucoseSolver;

} /* end namespace */

#endif /* __SATLIB_SATLIB_GLUCOSE_SOLVER_HPP */

