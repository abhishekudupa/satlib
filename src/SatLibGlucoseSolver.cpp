#include <SatLibGlucoseSolver.hpp>

namespace SatLib {

    const Glucose::lbool GlucoseTypeStruct::l_True = Glucose::l_True; 
    const Glucose::lbool GlucoseTypeStruct::l_False = Glucose::l_False;
    const Glucose::lbool GlucoseTypeStruct::l_Undef = Glucose::l_Undef;  
    const string GlucoseTypeStruct::SolverName = "GlucoseSolver";

    GlucoseTypeStruct::LitType GlucoseTypeStruct::mkLit(VarType Var, bool Sign)
    {
        return Glucose::mkLit(Var, Sign);
    }    

} /* end namespace */

