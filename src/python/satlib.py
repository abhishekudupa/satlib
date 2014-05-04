import ctypes
import satlibcore

# the enumerated types
VAR_TRUE = 0
VAR_FALSE = 1
VAR_UNDEF = 2

SOLVE_SAT = 0
SOLVE_UNSAT = 1
SOLVE_UNKNOWN = 2

SOLVER_MINISAT = 0
SOLVER_GLUCOSE = 1
SOLVER_LINGELING = 2
SOLVER_Z3 = 3

class SatLibSolverObj(ctypes.c_void_p):
    def __init__(self, obj):
        self._as_parameter_ = obj
        
    def from_param(obj):
        return obj

    def __del__(self):
        satlibcore.SatLibDeleteSolver(self)

    def GetName(self):
        return satlibcore.SatLibGetSolverName(self).value

    def Push(self):
        satlibcore.SatLibPush(self)

    def Pop(self, NumScopes = 1):
        satlibcore.SatLibPop(self, NumScopes)

    def GetNumScopes(self):
        return satlibcore.SatLibGetNumScopes(self).value

    def CreateAnd(self, Args):
        return satlibcore.SatLibCreateAnd(self, Args)

    def CreateOr(self, Args):
        return satlibcore.SatLibCreateOr(self, Args)

    def CreateNor(self, Args):
        return satlibcore.SatLibCreateNot(self, Args)

    def CreateNand(self, Args):
        return satlibcore.SatLibCreateNand(self, Args)

    def CreateNot(self, Arg):
        return satlibcore.SatLibCreateNot(self, Arg)

    def CreateVar(self, Name):
        return satlibcore.SatLibCreateVar(self, Name)

    def CreateBAnd(self, Arg1, Arg2):
        return satlibcore.SatLibCreateBAnd(self, Arg1, Arg2)

    def CreateBOr(self, Arg1, Arg2):
        return satlibcore.SatLibCreateBOr(self, Arg1, Arg2)
    
    def CreateBNor(self, Arg1, Arg2):
        return satlibcore.SatLibCreateBNor(self, Arg1, Arg2)

    def CreateBNand(self, Arg1, Arg2):
        return satlibcore.SatLibCreateBNand(self, Arg1, Arg2)

    def CreateBXor(self, Arg1, Arg2):
        return satlibcore.SatLibCreateBXor(self, Arg1, Arg2)

    def CreateBXnor(self, Arg1, Arg2):
        return satlibcore.SatLibCreateBXnor(self, Arg1, Arg2)
    
    def CreateImplies(self, Arg1, Arg2):
        return satlibcore.SatLibCreateImplies(self, Arg1, Arg2)

    def CreateIff(self, Arg1, Arg2):
        return satlibcore.SatLibCreateIff(self, Arg1, Arg2)

    def CreateTrue(self):
        return satlibcore.SatLibCreateTrue(self)

    def CreateFalse(self):
        return satlibcore.SatLibCreateFalse(self)

    def Assert(self, Arg):
        satlibcore.SatLibAssert(self, Arg)

    def GetAssertions(self):
        return satlibcore.SatLibGetAssertions(self)

    def Solve(self):
        return satlibcore.SatLibSolve(self)

    def GetModel(self):
        return satlibcore.SatLibGetModel(self)

class SatLibExpObj(ctypes.c_void_p):
    def __init__(self, obj):
        self._as_parameter_ = obj

    def from_param(obj):
        return obj

    def ToString(self):
        return satlibcore.SatLibPropToString(self)

class ModelEntry(ctypes.Structure):
    _fields_ = [ ("VarName", ctypes.POINTER(ctypes.c_char)),
                 ("VarValue", ctypes.c_int) ]


def CreateSolver(SolverType):
    return satlibcore.SatLibCreateSolver(SolverType)
