#include <SatLibInclude.hpp>
#include <SatLibMinisatSolver.hpp>
#include <SatLibGlucoseSolver.hpp>
#include <ExpressionBase.hpp>
#include <SatLibExceptions.hpp>

using namespace SatLib;

static inline void RegisterError(const exception& e)
{
    SatLibCLastErrorCode = 1;
    auto what = e.what();
    strcpy(SatLibCLastErrorMsg, what);
}

SLSolver* SatLibCreateSolver(SLSolverType SLSType) 
{
    try {
        if (SLSType == SATLIB_SOLVER_MINISAT) {
            return new SatLibMinisatSolver();
        } else if (SLSType == SATLIB_SOLVER_GLUCOSE) {
            return new SatLibGlucoseSolver();
        } else {
            fprintf(stderr, "Unknown Solver Type in SatLibCreateSolver()\n");
            return NULL;
        }
    } catch (exception& e) {
        RegisterError(e);
    }
    return NULL;
}

void SatLibDeleteSolver(SLSolver* Solver)
{
    try {
        delete Solver;
    } catch (exception& e) {
        RegisterError(e);
    }
}

const char* SatLibGetSolverName(const SLSolver* Solver)
{
    try {
        return Solver->GetName().c_str();
    } catch (exception& e) {
        RegisterError(e);
    }
    return NULL;
}

void SatLibPush(SLSolver* Solver)
{
    try {
        Solver->Push();
    } catch (exception& e) {
        RegisterError(e);
    }
}

void SatLibPop(SLSolver* Solver, u32 NumScopes)
{
    try {
        Solver->Pop(NumScopes);
    } catch (exception& e) {
        RegisterError(e);
    }
}

u32 SatLibGetNumScopes(SLSolver* Solver)
{
    try {
        return Solver->GetNumScopes();
    } catch (exception& e) {
        RegisterError(e);
    }
    return 0;
}

static inline PropVec CreateArgVec(PropFormula* Args, u32 NumArgs)
{
    PropVec Retval(NumArgs);
    for(u32 i = 0; i < NumArgs; ++i) {
        Retval[i] = Args[i];
    }
    return Retval;
}

PropFormula SatLibCreateAnd(SLSolver* Solver, PropFormula* Args, u32 NumArgs)
{
    try {
        auto&& Vec = CreateArgVec(Args, NumArgs);
        return Solver->CreateAnd(Vec);
    } catch (exception& e) {
        RegisterError(e);
    }
    return NULL;
}

PropFormula SatLibCreateOr(SLSolver* Solver, PropFormula* Args, u32 NumArgs)
{
    try {
        auto&& Vec = CreateArgVec(Args, NumArgs);
        return Solver->CreateOr(Vec);
    } catch (exception& e) {
        RegisterError(e);
    }
    return NULL;
}

PropFormula SatLibCreateNor(SLSolver* Solver, PropFormula* Args, u32 NumArgs)
{
    try {
        auto&& Vec = CreateArgVec(Args, NumArgs);
        return Solver->CreateNor(Vec);
    } catch (exception& e) {
        RegisterError(e);
    }
    return NULL;
}

PropFormula SatLibCreateNand(SLSolver* Solver, PropFormula* Args, u32 NumArgs)
{
    try {
        auto&& Vec = CreateArgVec(Args, NumArgs);
        return Solver->CreateNand(Vec);
    } catch (exception& e) {
        RegisterError(e);
    }
    return NULL;
}

PropFormula SatLibCreateNot(SLSolver* Solver, PropFormula Arg)
{
    try {
        return Solver->CreateNot(Arg);
    } catch (exception& e) {
        RegisterError(e);
    }
    return NULL;
}

PropFormula SatLibCreateVar(SLSolver* Solver, const char* VarName)
{
    try {
        return Solver->CreateVar(string(VarName));
    } catch (exception& e) {
        RegisterError(e);
    }
    return NULL;
}

PropFormula SatLibCreateBAnd(SLSolver* Solver, PropFormula Arg1, PropFormula Arg2)
{
    try {
        return Solver->CreateAnd(Arg1, Arg2);
    } catch (exception& e) {
        RegisterError(e);
    }
    return NULL;
}

PropFormula SatLibCreateBOr(SLSolver* Solver, PropFormula Arg1, PropFormula Arg2)
{
    try {
        return Solver->CreateOr(Arg1, Arg2);
    } catch (exception& e) {
        RegisterError(e);
    }
    return NULL;
}


PropFormula SatLibCreateBNor(SLSolver* Solver, PropFormula Arg1, PropFormula Arg2)
{
    try {
        return Solver->CreateNor(Arg1, Arg2);
    } catch (exception& e) {
        RegisterError(e);
    }
    return NULL;
}

PropFormula SatLibCreateBNand(SLSolver* Solver, PropFormula Arg1, PropFormula Arg2)
{
    try {
        return Solver->CreateNand(Arg1, Arg2);
    } catch(exception& e) {
        RegisterError(e);
    }
    return NULL;
}

PropFormula SatLibCreateBXor(SLSolver* Solver, PropFormula Arg1, PropFormula Arg2)
{
    try {
        return Solver->CreateXor(Arg1, Arg2);
    } catch (exception& e) {
        RegisterError(e);
    }
    return NULL;
}

PropFormula SatLibCreateBXnor(SLSolver* Solver, PropFormula Arg1, PropFormula Arg2)
{
    try {
        return Solver->CreateXnor(Arg1, Arg2);
    } catch (exception& e) {
        RegisterError(e);
    }
    return NULL;
}

PropFormula SatLibCreateImplies(SLSolver* Solver, PropFormula Arg1, PropFormula Arg2)
{
    try {
        return Solver->CreateImplies(Arg1, Arg2);
    } catch (exception& e) {
        RegisterError(e);
    }
    return NULL;
}

PropFormula SatLibCreateIff(SLSolver* Solver, PropFormula Arg1, PropFormula Arg2)
{
    try {
        return Solver->CreateIff(Arg1, Arg2);
    } catch (exception& e) {
        RegisterError(e);
    }
    return NULL;
}

PropFormula SatLibCreateTrue(SLSolver* Solver)
{
    try {
        return Solver->CreateTrue();
    } catch (exception& e) {
        RegisterError(e);
    }
    return NULL;
}

PropFormula SatLibCreateFalse(SLSolver* Solver)
{
    try {
        return Solver->CreateFalse();
    } catch (exception& e) {
        RegisterError(e);
    }
    return NULL;
}

void SatLibAssert(SLSolver* Solver, PropFormula Prop)
{
    try {
        Solver->Assert(Prop);
    } catch (exception& e) {
        RegisterError(e);
    }
}

void SatLibGetAssertions(SLSolver* Solver, PropFormula** Assertions, u32* NumAssertions)
{
    try {
        auto const& AssertStack = Solver->GetAssertions();
        *NumAssertions = AssertStack.size();
        
        *Assertions = new PropFormula[*NumAssertions];
        
        u32 CurAssertNum = 0;
        for(u32 i = 0; i < *NumAssertions; ++i) {
            (*Assertions)[CurAssertNum++] = AssertStack[i];
        }
        return;
    } catch (exception& e) {
        RegisterError(e);
    }
}

void SatLibFreeAssertionVec(SLSolver* Solver, PropFormula* Assertions)
{
    try {
        delete[] Assertions;
    } catch (exception& e) {
        RegisterError(e);
    }
}

SLResult SatLibSolve(SLSolver* Solver)
{
    try {
        return Solver->Solve();
    } catch (exception& e) {
        RegisterError(e);
    }
    return SOLVE_UNKNOWN;
}

void SatLibGetModel(SLSolver* Solver, ModelEntry** ModelPtr, u32* NumEntries)
{
    try {
        SatModel Model;
        Solver->GetModel(Model);
    
        *NumEntries = Model.size();
        
        u32 CurEntryNum = 0;
        *ModelPtr = new ModelEntry[*NumEntries];
        
        for(auto it = Model.begin(); it != Model.end(); ++it) {
            u32 NameLen = it->first.length() + 1;
            char* Name = new char[NameLen];
            memcpy(Name, it->first.c_str(), sizeof(char) * NameLen);
            (*ModelPtr)[CurEntryNum].VarName = Name;
            (*ModelPtr)[CurEntryNum++].VarValue = it->second;
        }
        return;
    } catch (exception& e) {
        RegisterError(e);
    }
}


void SatLibDeleteModel(SLSolver* Solver, ModelEntry* ModelPtr, u32 NumEntries)
{
    try {
        for (u32 i = 0; i < NumEntries; ++i) {
            delete[] ModelPtr[i].VarName;
        }
        delete[] ModelPtr;
    } catch (exception& e) {
        RegisterError(e);
    }
}

const char* SatLibPropToString(PropFormula Prop)
{
    try {
        auto Str = Prop->ToString();
        char* Retval = new char[Str.length() + 1];
        memcpy(Retval, Str.c_str(), sizeof(char) * (Str.length() + 1));
        return Retval;
    } catch (exception& e) {
        RegisterError(e);
        return NULL;
    }
}

void SatLibDeletePropString(const char* PropString)
{
    try {
        delete[] PropString;
    } catch(exception& e) {
        RegisterError(e);
    }
}

u32 SatLibGetLastError()
{
    return SatLibCLastErrorCode;
}

void SatLibResetErrors()
{
    SatLibCLastErrorCode = 0;
}

const char* SatLibGetLastErrorMessage()
{
    return SatLibCLastErrorMsg;
}
