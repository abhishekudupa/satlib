#include <SatLibInclude.hpp>
#include <iostream>
#include <ctype.h>
#include <sstream>
#include <iomanip>
#include <stdlib.h>

using namespace SatLib;
using namespace std;

int main(int argc, char* argv[])
{
    if(argc != 2) {
        cout << "Usage: " << argv[0] << " <NumDisjuncts>" << endl;
        exit(1);
    }
    
    u32 NumDisjuncts = atoi(argv[1]);
    SatLibSolverBase* Solver = SatLibSolverBase::CreateSolver(SATLIB_SOLVER_MINISAT);
    PropFormula AccExp = Solver->CreateFalse();
    for(u32 i = 0; i < NumDisjuncts; ++i) {
        ostringstream sstr;
        sstr << "A" << setw(6) << setfill('0') << i;
        PropFormula P1 = Solver->CreateVar(sstr.str());
        sstr.str("");
        sstr << "B" << setw(6) << setfill('0') << i;
        PropFormula P2 = Solver->CreateVar(sstr.str());
        AccExp = Solver->CreateOr(AccExp, Solver->CreateAnd(P1, P2));
    }

    Solver->Assert(AccExp);

    if(Solver->Solve() == SOLVE_SAT) {
        cout << "SAT!" << endl;
        SatModel Model;
        Solver->GetModel(Model);
        cout << "Model:" << endl;
        for(SatModel::const_iterator it = Model.begin(); it != Model.end(); ++it) {
            cout << it->first << " : " << (it->second == VAR_TRUE ? "True" : "False") << endl;
        }
    } else {
        cout << "UNSAT!" << endl;
    }

    delete Solver;

    // do an A & !A test asserting twice
    Solver = SatLibSolverBase::CreateSolver(SATLIB_SOLVER_MINISAT);
    auto AExp = Solver->CreateVar("A");
    auto NotAExp = Solver->CreateNot(AExp);
    Solver->Assert(AExp);
    auto Res = Solver->Solve();
    if(Res == SOLVE_SAT) {
        cout << "SAT" << endl;
        SatModel Model;
        Solver->GetModel(Model);
        cout << "Model:" << endl;
        for(SatModel::const_iterator it = Model.begin(); it != Model.end(); ++it) {
            cout << it->first << " : " << (it->second == VAR_TRUE ? "True" : "False") << endl;
        }
    } else {
        cout << "UNSAT" << endl;
    }

    Solver->Assert(NotAExp);
    Res = Solver->Solve();
    if(Res == SOLVE_SAT) {
        cout << "SAT" << endl;
        SatModel Model;
        Solver->GetModel(Model);
        cout << "Model:" << endl;
        for(SatModel::const_iterator it = Model.begin(); it != Model.end(); ++it) {
            cout << it->first << " : " << (it->second == VAR_TRUE ? "True" : "False") << endl;
        }
    } else {
        cout << "UNSAT" << endl;
    }
}
