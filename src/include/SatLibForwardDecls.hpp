#if !defined __SATLIB_SATLIB_FORWARD_DECLS_HPP
#define __SATLIB_SATLIB_FORWARD_DECLS_HPP

#include "SatLibCommon.hpp"
#include <unordered_map>
#include "sparsehash/dense_hash_set"

// offset for where aux vars begin
#define SATLIB_AUX_VAR_OFFSET ((u32)((u32)1 << 29))

namespace SatLib {

    /* Forward Decls */
    class ExpressionBase;
    class ExpressionPtrHash;
    class ExpressionPtrEquals;
    class ExpressionManager;
    class ExpressionVisitorBase;
    class ExpressionPrinter;
    class UIDGenerator;
    class AuxUIDGenerator;
    class Literal;

    enum ModelValue {
        VAR_TRUE = 0,
        VAR_FALSE = 1,
        VAR_UNDEF = 2,
        // pad
        VAR_SENTINEL = (1 << 30)
    };

    // The type of solvers
    enum SLSolverType {
        SATLIB_SOLVER_MINISAT = 0,
        SATLIB_SOLVER_GLUCOSE = 1,
        SATLIB_SOLVER_LINGELING = 2,
        SATLIB_SOLVER_Z3 = 3,
        // pad
        SATLIB_SOLVER_SENTINEL = (1 << 30)
    };

    /* Typedefs */
    typedef google::dense_hash_set<ExpressionBase*,
                                   ExpressionPtrHash,
                                   ExpressionPtrEquals> ExpHashSet;

    typedef vector<Literal> Clause;
    typedef unordered_map<string, ModelValue> SatModel;
    typedef const ExpressionBase* PropFormula;
    typedef vector<PropFormula> PropVec;

    typedef enum {
        EXP_AND,
        EXP_OR,
        EXP_XOR,
        EXP_XNOR,
        EXP_NAND,
        EXP_NOR,
        EXP_VAR,
        EXP_IMPLIES,
        EXP_IFF,
        // Constant Expressions
        EXP_TRUE,
        EXP_FALSE
    } ExpOperator;

    enum SolveResult {
        SOLVE_SAT = 0,
        SOLVE_UNSAT = 1,
        SOLVE_UNKNOWN = 2,
        SOLVE_SENTINEL = (1 << 30)
    };

    ostream& operator << (ostream& str, const ExpressionBase& Exp);

} /* End namespace */

#endif /* __SATLIB_SATLIB_FORWARD_DECLS_HPP */

