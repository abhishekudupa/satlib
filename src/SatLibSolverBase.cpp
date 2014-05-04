#include <SatLibSolverBase.hpp>
#include <ExpressionManager.hpp>
#include <SatLibMinisatSolver.hpp>
#include <SatLibGlucoseSolver.hpp>
#include <SatLibExceptions.hpp>
#include <SatLibLingelingSolver.hpp>
#include <SatLibZ3Solver.hpp>

namespace SatLib {

    SatLibSolverBase::SatLibSolverBase(const string& SolverName)
        : SolverName(SolverName)
    {
        Mgr = new ExpressionManager();
    }

    SatLibSolverBase::~SatLibSolverBase()
    {
        Assertions.clear();
        delete Mgr;
    }

    const string& SatLibSolverBase::GetName() const
    {
        return SolverName;
    }

    void SatLibSolverBase::Push()
    {
        // Remember the size of the assertion vector
        BacktrackPoints.push_back(Assertions.size());
    }

    void SatLibSolverBase::Pop(u32 NumScopes)
    {
        if (NumScopes == 0) {
            return;
        }
        i32 BacktrackIndex = BacktrackPoints.size() - NumScopes;
        if (BacktrackIndex < 0) {
            throw SatLibExceptionBase("Error, tried to pop too many scopes");
        }
        // resize the vector
        Assertions.resize(BacktrackPoints[BacktrackIndex]);
        BacktrackPoints.resize(BacktrackIndex + 1);
        Mgr->Pop(NumScopes);
    }

    u32 SatLibSolverBase::GetNumScopes() const
    {
        return BacktrackPoints.size();
    }

    PropFormula SatLibSolverBase::CreateAnd(const PropVec& Args)
    {
        return Mgr->CreateAnd(Args);
    }

    PropFormula SatLibSolverBase::CreateOr(const PropVec& Args)
    {
        return Mgr->CreateOr(Args);
    }

    PropFormula SatLibSolverBase::CreateNot(PropFormula Arg)
    {
        return Mgr->CreateNot(Arg);
    }

    PropFormula SatLibSolverBase::CreateNand(const PropVec& Args)
    {
        return Mgr->CreateNand(Args);
    }

    PropFormula SatLibSolverBase::CreateNor(const PropVec& Args)
    {
        return Mgr->CreateNor(Args);
    }

    PropFormula SatLibSolverBase::CreateVar(const string& VarName)
    {
        return Mgr->CreateVar(VarName);
    }
    
    PropFormula SatLibSolverBase::CreateAnd(PropFormula Arg1, PropFormula Arg2)
    {
        return Mgr->CreateAnd(Arg1, Arg2);
    }

    PropFormula SatLibSolverBase::CreateOr(PropFormula Arg1, PropFormula Arg2)
    {
        return Mgr->CreateOr(Arg1, Arg2);
    }

    PropFormula SatLibSolverBase::CreateXor(PropFormula Arg1, PropFormula Arg2)
    {
        return Mgr->CreateXor(Arg1, Arg2);
    }

    PropFormula SatLibSolverBase::CreateXnor(PropFormula Arg1, PropFormula Arg2)
    {
        return Mgr->CreateXnor(Arg1, Arg2);
    }

    PropFormula SatLibSolverBase::CreateNand(PropFormula Arg1, PropFormula Arg2)
    {
        return Mgr->CreateNand(Arg1, Arg2);
    }

    PropFormula SatLibSolverBase::CreateNor(PropFormula Arg1, PropFormula Arg2)
    {
        return Mgr->CreateNor(Arg1, Arg2);
    }

    PropFormula SatLibSolverBase::CreateImplies(PropFormula Arg1, PropFormula Arg2)
    {
        return Mgr->CreateImplies(Arg1, Arg2);
    }

    PropFormula SatLibSolverBase::CreateIff(PropFormula Arg1, PropFormula Arg2)
    {
        return Mgr->CreateIff(Arg1, Arg2);
    }

    PropFormula SatLibSolverBase::CreateFalse()
    {
        return Mgr->CreateFalse();
    }

    PropFormula SatLibSolverBase::CreateTrue()
    {
        return Mgr->CreateTrue();
    }

    void SatLibSolverBase::Assert(PropFormula Prop)
    {
        Assertions.push_back(Prop);
    }

    const vector<PropFormula>& SatLibSolverBase::GetAssertions()
    {
        return Assertions;
    }

    SatLibSolverBase* SatLibSolverBase::CreateSolver(SLSolverType Type)
    {
        switch(Type) {
        case SATLIB_SOLVER_MINISAT:
            return new SatLibMinisatSolver();
        case SATLIB_SOLVER_GLUCOSE:
            return new SatLibGlucoseSolver();
        case SATLIB_SOLVER_LINGELING:
            return new SatLibLingelingSolver("liblgl-aqw.so");
        case SATLIB_SOLVER_Z3:
            return new SatLibZ3Solver();
        default:
            throw SatLibExceptionBase("Unknown solver type in SatLibSolverBase::CreateSolver()");
        }
        return NULL;
    }

} /* End namespace */

