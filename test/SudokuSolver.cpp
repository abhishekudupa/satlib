#include <SatLibInclude.hpp>
#include <iostream>
#include <ctype.h>
#include <sstream>
#include <iomanip>
#include <stdlib.h>
#include <vector>
#include <tinyxml2.h>
#include <math.h>

using namespace SatLib;
using namespace std;
using namespace tinyxml2;

static inline PropFormula AtMostOne(const PropVec& Props, SLSolver* Solver)
{
    const u32 NumProps = Props.size();
    assert(NumProps >= 2);

    PropFormula Retval = NULL;
    for(u32 i = 0; i < NumProps; ++i) {
        for(u32 j = i + 1; j < NumProps; ++j) {
            if(Retval == NULL) {
                Retval = Solver->CreateNand(Props[i], Props[j]);
            } else {
                Retval = Solver->CreateAnd(Retval, Solver->CreateNand(Props[i], Props[j]));
            }
        }
    }
    return Retval;
}

static inline PropFormula ExactlyOne(const PropVec& Props, SLSolver* Solver)
{
    const u32 NumProps = Props.size();
    assert(NumProps >= 2);

    PropFormula Retval = AtMostOne(Props, Solver);
    // Add the constraint that one of them MUST be true
    PropFormula Exactly = Solver->CreateOr(Props);
    return Solver->CreateAnd(Retval, Exactly);
}

class SudokuGrid
{
private:
    vector<vector<i32> > Grid;
    vector<vector<i32> > SolvedGrid;
    const u32 Size;
    vector<vector<vector<PropFormula> > > VarVec;
    vector<vector<vector<string> > > VarNames;

    // Helper methods
    inline void InitVarVec(SLSolver* Solver);
    inline void InitVarNames();
    inline string GenerateVarName(u32 Row, u32 Col, u32 Val);
    inline void AddBaseConstraints(SLSolver* SATSolver);
    inline void AddConstraints(SLSolver* Solver);
    inline void LiftSolution(SLSolver* Solver);

public:
    SudokuGrid(u32 Size);
    ~SudokuGrid();
    void AddConstraint(u32 Row, u32 Col, u32 Value);
    bool Solve(SLSolver* SATSolver);
    void PrintGrid(ostream& str);
    void PrintSolved(ostream& str);
};

SudokuGrid::SudokuGrid(u32 Size)
    : Size(Size)
{
    for(u32 i = 0; i < Size * Size; ++i) {
        Grid.push_back(vector<i32>(Size * Size, -1));
        SolvedGrid.push_back(vector<i32>(Size * Size, -1));
    }
    InitVarNames();
}

SudokuGrid::~SudokuGrid()
{
    // Nothing here
}

void SudokuGrid::AddConstraint(u32 Row, u32 Col, u32 Value)
{
    assert(Row < Size * Size);
    assert(Col < Size * Size);
    assert(Value < Size * Size);

    Grid[Row][Col] = Value;
}


void SudokuGrid::PrintGrid(ostream& str)
{
    for(u32 i = 0; i < Size * Size; ++i) {
        if(i > 0 && (i % Size) == 0) {
            str << endl;
        }
        for(u32 j = 0; j < Size * Size; ++j) {
            if(j > 0 && (j % Size) == 0) {
                str << "    ";
            }
            str << setw(2) << setfill('0') << (Grid[i][j] == -1 ? 0 : Grid[i][j]) << " ";
        }
        str << endl;
    }
    str << endl;
}

void SudokuGrid::PrintSolved(ostream& str)
{
    for(u32 i = 0; i < Size * Size; ++i) {
        if(i > 0 && (i % Size) == 0) {
            str << endl;
        }
        for(u32 j = 0; j < Size * Size; ++j) {
            if(j > 0 && (j % Size) == 0) {
                str << "    ";
            }
            str << setw(2) << setfill('0') << SolvedGrid[i][j] << " ";
        }
        str << endl;
    }
    str << endl;
}

inline void SudokuGrid::InitVarNames()
{
    VarNames = vector<vector<vector<string> > >
        (Size * Size, 
         vector<vector<string> >
         (Size * Size, 
          vector<string>(Size * Size, "")));

    for(u32 i = 0; i < Size * Size; ++i) {
        for(u32 j = 0; j < Size * Size; ++j) {
            for(u32 k = 0; k < Size * Size; ++k) {
                VarNames[i][j][k] = GenerateVarName(i, j, k);
            }
        }
    }
}

inline void SudokuGrid::InitVarVec(SLSolver* Solver)
{
    VarVec = vector<vector<vector<PropFormula> > >
        (Size * Size,
         vector<vector<PropFormula> >
         (Size * Size,
          vector<PropFormula>(Size * Size, NULL)));

    for(u32 i = 0; i < Size * Size; ++i) {
        for(u32 j = 0; j < Size * Size; ++j) {
            for(u32 k = 0; k < Size * Size; ++k) {
                VarVec[i][j][k] = Solver->CreateVar(VarNames[i][j][k]);
            }
        }
    }
}

inline string SudokuGrid::GenerateVarName(u32 Row, u32 Col, u32 Val)
{
    assert(Row < Size * Size);
    assert(Col < Size * Size);
    assert(Val < Size * Size);

    ostringstream sstr;
    sstr << "v_" << setw(2) << setfill('0') << Row
         << "_" << setw(2) << setfill('0') << Col
         << "_" << setw(2) << setfill('0') << Val;
    return sstr.str();
}

// Add the base constraints to the solver
/*
  We have 'Size' boolean variables per cell
  corresponding to a one hot encoding
  
  Variable names are encoded as v_<row>_<col>_<val>
  
  Constraints:
  1. Cell constraints: for all row and col, exactly one of v_<row>_<col>_<val> is true
  2. Row constraints: for all row and val, exactly one of v_<row>_<col>_<val> is true
  3. Col constraints: for all col and val, exactly one of v_<row>_<col>_<val> is true
  4. Block constraints for all val, exactly one of v_<p1 * Size + q1>_<p2 * Size + q2>_<val> 
     is true, 0 <= p1, q1, p2, q2 < Size
*/

inline void SudokuGrid::AddBaseConstraints(SLSolver* Solver)
{
    // Cell constraints
    // Collect over all the vals for each row and col
    for(u32 i = 0; i < Size * Size; ++i) {
        for(u32 j = 0; j < Size * Size; ++j) {
            vector<PropFormula> PropVec;
            for(u32 k = 0; k < Size * Size; ++k) {
                PropVec.push_back(VarVec[i][j][k]);
            }
            // Assert exactly one constraint
            PropFormula Assertion = ExactlyOne(PropVec, Solver);
            // cout << "Asserting: " << *Assertion << endl;
            Solver->Assert(Assertion);
        }
    }

    // Row constraints
    // Collect over all cols for each row and val
    for(u32 i = 0; i < Size * Size; ++i) {
        for(u32 k = 0; k < Size * Size; ++k) {
            vector<PropFormula> PropVec;
            for(u32 j = 0; j < Size * Size; ++j) {
                PropVec.push_back(VarVec[i][j][k]);
            }
            // Assert exactly one constraint
            PropFormula Assertion = ExactlyOne(PropVec, Solver);
            // cout << "Asserting: " << *Assertion << endl;
            Solver->Assert(Assertion);
        }
    }

    // Col constraints
    // Collect over all rows for each col and val
    for(u32 j = 0; j < Size * Size; ++j) {
        for(u32 k = 0; k < Size * Size; ++k) {
            vector<PropFormula> PropVec;
            for(u32 i = 0; i < Size * Size; ++i) {
                PropVec.push_back(VarVec[i][j][k]);
            }
            // Assert exactly one constraint
            PropFormula Assertion = ExactlyOne(PropVec, Solver);
            // cout << "Asserting: " << *Assertion << endl;
            Solver->Assert(Assertion);
        }
    }

    // Block constraints
    for(u32 k = 0; k < Size * Size; ++k) {
        // Select a block
        for(u32 p1 = 0; p1 < Size; ++p1) {
            for(u32 p2 = 0; p2 < Size; ++p2) {
                vector<PropFormula> PropVec;
                for(u32 q1 = 0; q1 < Size; ++q1) {
                    for(u32 q2 = 0; q2 < Size; ++q2) {
                        PropVec.push_back(VarVec[p1 * Size + q1][p2 * Size + q2][k]);
                    }
                }
                PropFormula Assertion = ExactlyOne(PropVec, Solver);
                // cout << "Asserting: " << *Assertion << endl;
                Solver->Assert(Assertion);
            }
        }
    }
}

inline void SudokuGrid::AddConstraints(SLSolver* Solver)
{
    // Add the constraints on the vars for each variable
    for(u32 i = 0; i < Size * Size; ++i) {
        for(u32 j = 0; j < Size * Size; ++j) {
            if(Grid[i][j] != -1) {
                // cout << "Asserting: " << *(VarVec[i][j][Grid[i][j]]) << endl;
                Solver->Assert(VarVec[i][j][Grid[i][j]]);
            }
        }
    }
}

inline void SudokuGrid::LiftSolution(SLSolver* Solver)
{
    SatModel Model;
    Solver->GetModel(Model);

    for(u32 i = 0; i < Size * Size; ++i) {
        for(u32 j = 0; j < Size * Size; ++j) {
            i32 Solution = -1;
            for(u32 k = 0; k < Size * Size; ++k) {
                if(Model[VarNames[i][j][k]] == VAR_TRUE) {
                    Solution = k;
                }
            }
            if(Solution == -1) {
                cout << "No solution exists!" << endl;
                exit(1);
            }
            SolvedGrid[i][j] = Solution;
        }
    }
}

bool SudokuGrid::Solve(SLSolver* SATSolver)
{
    SATSolver->Push();
    // cout << "Preprocessing to SAT..." << endl;
    InitVarVec(SATSolver);
    AddBaseConstraints(SATSolver);
    AddConstraints(SATSolver);
    // cout << "Done preprocessing, going to solve..." << endl;
    if(SATSolver->Solve() != SOLVE_SAT) {
        cout << "No solution exists!" << endl;
        return false;
    }
    LiftSolution(SATSolver);
    return true;
}

// Utility function to parse the values
static inline void ParseValues(const string& Values, SudokuGrid* Grid,
                               u32 Size)
{
    for(u32 i = 0; i < Size * Size; ++i) {
        for(u32 j = 0; j < Size * Size; ++j) {
            if(Values[i * Size * Size + j] != '_') {
                u32 Val = Values[i * Size * Size + j] - 'b';
                Grid->AddConstraint(i, j, Val);
            }
        }
    }
}

int main(int argc, char* argv[])
{
    if(argc != 2) {
        cout << "Usage: " << argv[0] << " <Problem Definition>" << endl;
        exit(1);
    }

    XMLDocument Doc;
    Doc.LoadFile(argv[1]);
    XMLElement* Puzzle = Doc.FirstChildElement("ksudoku")->FirstChildElement("game")->FirstChildElement("puzzle");
    u32 Size = Puzzle->FirstChildElement("graph")->UnsignedAttribute("order");
    Size = (u32)sqrt(Size);
    string Values = Puzzle->FirstChildElement("values")->GetText();
    assert(Values.length() == (Size * Size * Size * Size));

    SudokuGrid* Grid = new SudokuGrid(Size);
    SLSolver* Solver = SLSolver::CreateSolver(SATLIB_SOLVER_Z3);
    ParseValues(Values, Grid, Size);
    cout << "Initial Grid:" << endl;
    Grid->PrintGrid(cout);
    
    Grid->Solve(Solver);
    cout << "Solved Grid:" << endl;
    
    Grid->PrintSolved(cout);

    delete Grid;
    delete Solver;
}
