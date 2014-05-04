import sys, os, ctypes
import satlib
import pprint, copy

class SatLibException(Exception):
    def __init__(self, Info):
        self.Info = Info
    def __str__(self):
        return self.Info
    def __repr__(self):
        return 'SatLibException: Info = %s' % self.Info

def GetLibPath():
    Dir = os.path.dirname(os.path.abspath(__file__))
    LibPath = os.path.join(Dir, 'libsatlib.so')
    return LibPath

Lib = None

def GetLib():
    global Lib
    if (Lib == None):
        Init(GetLibPath())
        return Lib
    else:
        return Lib

def Init(Path):
    global Lib
    
    Lib = ctypes.CDLL(Path)
    Lib.SatLibCreateSolver.restype = satlib.SatLibSolverObj
    Lib.SatLibCreateSolver.argtypes = [ ctypes.c_int ]
    
    Lib.SatLibDeleteSolver.argtypes = [ satlib.SatLibSolverObj ]

    Lib.SatLibGetSolverName.restype = ctypes.c_char_p
    Lib.SatLibGetSolverName.argtypes = [ satlib.SatLibSolverObj ]

    Lib.SatLibPush.argtypes = [ satlib.SatLibSolverObj ]
    Lib.SatLibPop.argtypes = [ satlib.SatLibSolverObj, ctypes.c_uint ]
    
    Lib.SatLibGetNumScopes.restype = ctypes.c_uint
    Lib.SatLibGetNumScopes.argtypes = [ satlib.SatLibSolverObj ]

    Lib.SatLibCreateAnd.restype = satlib.SatLibExpObj
    Lib.SatLibCreateAnd.argtypes = [ satlib.SatLibSolverObj, ctypes.POINTER(satlib.SatLibExpObj), ctypes.c_uint ]

    Lib.SatLibCreateOr.restype = satlib.SatLibExpObj
    Lib.SatLibCreateOr.argtypes = [ satlib.SatLibSolverObj, ctypes.POINTER(satlib.SatLibExpObj), ctypes.c_uint ]

    Lib.SatLibCreateNot.restype = satlib.SatLibExpObj
    Lib.SatLibCreateNot.argtypes = [ satlib.SatLibSolverObj, satlib.SatLibExpObj ]

    Lib.SatLibCreateNand.restype = satlib.SatLibExpObj
    Lib.SatLibCreateNand.argtypes = [ satlib.SatLibSolverObj, ctypes.POINTER(satlib.SatLibExpObj), ctypes.c_uint ]

    Lib.SatLibCreateNor.restype = satlib.SatLibExpObj
    Lib.SatLibCreateNor.argtypes = [ satlib.SatLibSolverObj, ctypes.POINTER(satlib.SatLibExpObj), ctypes.c_uint ]
    
    Lib.SatLibCreateVar.restype = satlib.SatLibExpObj
    Lib.SatLibCreateVar.argtypes = [ satlib.SatLibSolverObj, ctypes.c_char_p ]

    Lib.SatLibCreateBAnd.restype = satlib.SatLibExpObj
    Lib.SatLibCreateBAnd.argtypes = [ satlib.SatLibSolverObj, satlib.SatLibExpObj, satlib.SatLibExpObj ]

    Lib.SatLibCreateBOr.restype = satlib.SatLibExpObj
    Lib.SatLibCreateBOr.argtypes = [ satlib.SatLibSolverObj, satlib.SatLibExpObj, satlib.SatLibExpObj ]

    Lib.SatLibCreateBNor.restype = satlib.SatLibExpObj
    Lib.SatLibCreateBNor.argtypes = [ satlib.SatLibSolverObj, satlib.SatLibExpObj, satlib.SatLibExpObj ]

    Lib.SatLibCreateBNand.restype = satlib.SatLibExpObj
    Lib.SatLibCreateBNand.argtypes = [ satlib.SatLibSolverObj, satlib.SatLibExpObj, satlib.SatLibExpObj ]

    Lib.SatLibCreateBXor.restype = satlib.SatLibExpObj
    Lib.SatLibCreateBXor.argtypes = [ satlib.SatLibSolverObj, satlib.SatLibExpObj, satlib.SatLibExpObj ]

    Lib.SatLibCreateBXnor.restype = satlib.SatLibExpObj
    Lib.SatLibCreateBXnor.argtypes = [ satlib.SatLibSolverObj, satlib.SatLibExpObj, satlib.SatLibExpObj ]

    Lib.SatLibCreateImplies.restype = satlib.SatLibExpObj
    Lib.SatLibCreateImplies.argtypes = [ satlib.SatLibSolverObj, satlib.SatLibExpObj, satlib.SatLibExpObj ]

    Lib.SatLibCreateIff.restype = satlib.SatLibExpObj
    Lib.SatLibCreateIff.argtypes = [ satlib.SatLibSolverObj, satlib.SatLibExpObj, satlib.SatLibExpObj ]
    
    Lib.SatLibCreateFalse.restype = satlib.SatLibExpObj
    Lib.SatLibCreateFalse.argtypes = [ satlib.SatLibSolverObj ]

    Lib.SatLibCreateTrue.restype = satlib.SatLibExpObj
    Lib.SatLibCreateTrue.argtypes = [ satlib.SatLibSolverObj ]

    Lib.SatLibAssert.argtypes = [ satlib.SatLibSolverObj, satlib.SatLibExpObj ]
    
    Lib.SatLibGetAssertions.argtypes = [ satlib.SatLibSolverObj, 
                                         ctypes.POINTER(ctypes.POINTER(satlib.SatLibExpObj)), 
                                         ctypes.POINTER(ctypes.c_uint) ]

    Lib.SatLibFreeAssertionVec.argtypes = [ satlib.SatLibSolverObj, ctypes.POINTER(satlib.SatLibExpObj) ]

    Lib.SatLibSolve.restype = ctypes.c_int
    Lib.SatLibSolve.argtypes = [ satlib.SatLibSolverObj ]

    Lib.SatLibGetModel.argtypes = [ satlib.SatLibSolverObj, 
                                    ctypes.POINTER(ctypes.POINTER(satlib.ModelEntry)), 
                                    ctypes.POINTER(ctypes.c_uint) ]

    Lib.SatLibDeleteModel.argtypes = [ satlib.SatLibSolverObj, ctypes.POINTER(satlib.ModelEntry), ctypes.c_uint ]

    Lib.SatLibPropToString.argtypes = [ satlib.SatLibExpObj ]
    Lib.SatLibPropToString.restype = ctypes.POINTER(ctypes.c_char)

    Lib.SatLibDeletePropString.argtypes = [ ctypes.POINTER(ctypes.c_char) ]

    Lib.SatLibGetLastError.argtypes = []
    Lib.SatLibGetLastError.restype = ctypes.c_uint

    Lib.SatLibResetErrors.argtypes = []
    Lib.SatLibGetLastErrorMessage.argtypes = []
    Lib.SatLibGetLastErrorMessage.restype = ctypes.POINTER(ctypes.c_char)


def CheckError():
    Err = (GetLib().SatLibGetLastError())
    if (Err != 0):
        MsgP = (GetLib().SatLibGetLastErrorMessage())
        ErrMsg = ctypes.cast(MsgP, ctypes.c_char_p).value
        GetLib().SatLibResetErrors()
        raise SatLibException(ErrMsg)
    else:
        return

# wrappers
def SatLibCreateSolver(SolverType):
    r = GetLib().SatLibCreateSolver(SolverType)
    CheckError()
    return r

def SatLibDeleteSolver(Solver):
    GetLib().SatLibDeleteSolver(Solver)
    CheckError()

def SatLibGetSolverName(Solver):
    r = GetLib().SatLibGetSolverName(Solver)
    CheckError()
    return r

def SatLibPush(Solver):
    GetLib().SatLibPush(Solver)
    CheckError()

def SatLibPop(Solver, NumScopes):
    r = GetLib().SatLibPop(Solver, NumScopes)
    CheckError()
    return r

def SatLibGetNumScopes(Solver):
    r = GetLib().SatLibGetNumScopes(Solver)
    CheckError()
    return r

def ExpListToArray(Args):
    Arr = (satlib.SatLibExpObj * len(Args)) ()
    Size = len(Args)
    for i in range(Size):
        Arr[i] = Args[i]
    return Arr, Size

def SatLibCreateAnd(Solver, Conjuncts):
    Args, Size = ExpListToArray(Conjuncts);
    r = GetLib().SatLibCreateAnd(Solver, Args, Size)
    CheckError()
    return r

def SatLibCreateOr(Solver, Disjuncts):
    Args, Size = ExpListToArray(Disjuncts)
    r = GetLib().SatLibCreateOr(Solver, Args, Size)
    CheckError()
    return r

def SatLibCreateNor(Solver, ArgList):
    Args, Size = ExpListToArray(ArgList)
    r = GetLib().SatLibCreateNor(Solver, Args, Size)
    CheckError()
    return r

def SatLibCreateNand(Solver, ArgList):
    Args, Size = ExpListToArray(ArgList)
    r = GetLib().SatLibCreateNand(Solver, Args, Size)
    CheckError()
    return r

def SatLibCreateNot(Solver, Exp):
    r = GetLib().SatLibCreateNot(Solver, Exp)
    CheckError()
    return r

def SatLibCreateVar(Solver, Name):
    r = GetLib().SatLibCreateVar(Solver, ctypes.c_char_p(Name))
    CheckError()
    return r

def SatLibCreateBAnd(Solver, Arg1, Arg2):
    r = GetLib().SatLibCreateBAnd(Solver, Arg1, Arg2)
    CheckError()
    return r

def SatLibCreateBOr(Solver, Arg1, Arg2):
    r = GetLib().SatLibCreateBOr(Solver, Arg1, Arg2)
    CheckError()
    return r

def SatLibCreateBNand(Solver, Arg1, Arg2):
    r = GetLib().SatLibCreateBNand(Solver, Arg1, Arg2)
    CheckError()
    return r

def SatLibCreateBNor(Solver, Arg1, Arg2):
    r = GetLib().SatLibCreateBNor(Solver, Arg1, Arg2)
    CheckError()
    return r

def SatLibCreateBXor(Solver, Arg1, Arg2):
    r = GetLib().SatLibCreateBXor(Solver, Arg1, Arg2)
    CheckError()
    return r

def SatLibCreateBXnor(Solver, Arg1, Arg2):
    r = GetLib().SatLibCreateBXnor(Solver, Arg1, Arg2)
    CheckError()
    return r

def SatLibCreateImplies(Solver, Arg1, Arg2):
    r = GetLib().SatLibCreateImplies(Solver, Arg1, Arg2)
    CheckError()
    return r

def SatLibCreateIff(Solver, Arg1, Arg2):
    r = GetLib().SatLibCreateIff(Solver, Arg1, Arg2)
    CheckError()
    return r

def SatLibCreateFalse(Solver):
    r = GetLib().SatLibCreateFalse(Solver)
    CheckError()
    return r

def SatLibCreateTrue(Solver):
    r = GetLib().SatLibCreateTrue(Solver)
    CheckError()
    return r

def SatLibAssert(Solver, Arg):
    GetLib().SatLibAssert(Solver, Arg)
    CheckError()
    return

def SatLibFreeAssertionVec(Solver, AssertVec):
    GetLib().SatLibFreeAssertionVec(Solver, AssertVec)
    CheckError()
    return

def SatLibGetAssertions(Solver):
    AssertVec = ctypes.POINTER(satlib.SatLibExpObj)()
    NumAssertsPtr = ctypes.pointer(ctypes.c_uint(0))
    GetLib().SatLibGetAssertions(Solver, AssertVec, NumAssertsPtr);
    CheckError()
    # we now push the assertions into a list
    Retval = []
    for i in range(NumAssertsPtr.contents.value):
        Retval.append(copy.deepcopy(AssertVec[i]))
    # free the assertion vector before returning
    SatLibFreeAssertionVec(Solver, AssertVec)
    CheckError()
    return Retval

def SatLibSolve(Solver):
    r = GetLib().SatLibSolve(Solver)
    CheckError()
    return r

def SatLibDeleteModel(Solver, Model, NumEntries):
    GetLib().SatLibDeleteModel(Solver, Model, NumEntries)
    CheckError()
    return

def SatLibGetModel(Solver):
    ModelPtr = ctypes.pointer(ctypes.POINTER(satlib.ModelEntry)())
    NumEntriesPtr = ctypes.pointer(ctypes.c_uint(0))
    GetLib().SatLibGetModel(Solver, ModelPtr, NumEntriesPtr)
    CheckError()
    # process the model into a map
    Retval = {}
    for i in range(NumEntriesPtr.contents.value):
        Retval[ctypes.cast(ModelPtr.contents[i].VarName, ctypes.c_char_p).value] = ModelPtr.contents[i].VarValue
    # free the model object
    SatLibDeleteModel(Solver, ModelPtr.contents, NumEntriesPtr.contents)
    CheckError()
    return Retval

def SatLibDeletePropString(PropString):
    GetLib().SatLibDeletePropString(PropString);
    CheckError()
    return

def SatLibPropToString(Exp):
    CharP = GetLib().SatLibPropToString(Exp)
    CheckError()
    Retval = ctypes.cast(CharP, ctypes.c_char_p).value
    SatLibDeletePropString(CharP)
    return Retval
    
