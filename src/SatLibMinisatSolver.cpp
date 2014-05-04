#include <SatLibMinisatSolver.hpp>

namespace SatLib {

    const Minisat::lbool MinisatTypeStruct::l_True = Minisat::l_True; 
    const Minisat::lbool MinisatTypeStruct::l_False = Minisat::l_False;
    const Minisat::lbool MinisatTypeStruct::l_Undef = Minisat::l_Undef;  
    const string MinisatTypeStruct::SolverName = "MinisatSolver";

    MinisatTypeStruct::LitType MinisatTypeStruct::mkLit(VarType Var, bool Sign)
    {
        return Minisat::mkLit(Var, Sign);
    }    

} /* end namespace */

