#if !defined __SATLIB_INCLUDE_HPP
#define __SATLIB_INCLUDE_HPP

#include "SatLibForwardDecls.hpp"
#include "SatLibSolverBase.hpp"
#include "SatLibCommon.hpp"


// We provide a C interface in addition to the C++ interface
// already provided.

extern "C" {
    
    using namespace SatLib;

    // Error messages
    const u32 SATLIB_C_MAX_ERROR_LEN = 1024;
    char SatLibCLastErrorMsg[SATLIB_C_MAX_ERROR_LEN];
    u32 SatLibCLastErrorCode = 0;

    typedef SatLib::ModelValue ModelValue;
    typedef SatLib::SLSolverType SLSolverType;

    typedef struct {
        char* VarName;
        ModelValue VarValue;
    } ModelEntry;

    typedef SatLib::SatLibSolverBase SLSolver;
    typedef SatLib::PropFormula PropFormula;
    typedef SatLib::u32 u32;
    typedef SatLib::SolveResult SLResult;
    
    SLSolver* SatLibCreateSolver(SLSolverType SLSType);
    void SatLibDeleteSolver(SLSolver* Solver);
    
    const char* SatLibGetSolverName(const SLSolver* Solver);
    
    void SatLibPush(SLSolver* Solver);
    void SatLibPop(SLSolver* Solver, u32 NumScopes);
    u32 SatLibGetNumScopes(SLSolver* Solver);
    
    PropFormula SatLibCreateAnd(SLSolver* Solver, PropFormula* Args, u32 NumArgs);
    PropFormula SatLibCreateOr(SLSolver* Solver, PropFormula* Args, u32 NumArgs);
    PropFormula SatLibCreateNot(SLSolver* Solver, PropFormula Arg);
    PropFormula SatLibCreateNand(SLSolver* Solver, PropFormula* Args, u32 NumArgs);
    PropFormula SatLibCreateNor(SLSolver* Solver, PropFormula* Args, u32 NumArgs);
    PropFormula SatLibCreateVar(SLSolver* Solver, const char* VarName);
    
    PropFormula SatLibCreateBAnd(SLSolver* Solver, PropFormula Arg1, PropFormula Arg2);
    PropFormula SatLibCreateBOr(SLSolver* Solver, PropFormula Arg1, PropFormula Arg2);
    PropFormula SatLibCreateBNor(SLSolver* Solver, PropFormula Arg1, PropFormula Arg2);
    PropFormula SatLibCreateBNand(SLSolver* Solver, PropFormula Arg1, PropFormula Arg2);
    PropFormula SatLibCreateBXor(SLSolver* Solver, PropFormula Arg1, PropFormula Arg2);
    PropFormula SatLibCreateBXnor(SLSolver* Solver, PropFormula Arg1, PropFormula Arg2);
    PropFormula SatLibCreateImplies(SLSolver* Solver, PropFormula Arg1, PropFormula Arg2);
    PropFormula SatLibCreateIff(SLSolver* Solver, PropFormula Arg1, PropFormula Arg2);

    PropFormula SatLibCreateFalse(SLSolver* Solver);
    PropFormula SatLibCreateTrue(SLSolver* Solver);

    void SatLibAssert(SLSolver* Solver, PropFormula Prop);
    void SatLibGetAssertions(SLSolver* Solver, PropFormula** Assertions, u32* NumAssertions);
    void SatLibFreeAssertionVec(SLSolver* Solver, PropFormula* Assertions);

    SLResult SatLibSolve(SLSolver* Solver);
    void SatLibGetModel(SLSolver* Solver, ModelEntry** ModelPtr, u32* NumEntries);
    void SatLibDeleteModel(SLSolver* Solver, ModelEntry* ModelPtr, u32 NumEntries);

    // Printing routines
    const char* SatLibPropToString(PropFormula Prop);
    void SatLibDeletePropString(const char* PropString);
    
    // Error handling
    u32 SatLibGetLastError();
    void SatLibResetErrors();
    const char* SatLibGetLastErrorMessage();
    

} /* end extern linkage */

#endif /* __SATLIB_INCLUDE_HPP */

