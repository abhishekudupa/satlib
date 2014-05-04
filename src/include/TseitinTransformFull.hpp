#if !defined __SATLIB_TSEITIN_TRANSFORM_FULL_HPP
#define __SATLIB_TSEITIN_TRANSFORM_FULL_HPP

#include "SatLibForwardDecls.hpp"
#include "CNFConverterBase.hpp"
#include "Literal.hpp"
#include "sparsehash/dense_hash_map"

namespace SatLib {

    // An optimized Tseitin Transform. 
    // Implements:
    // 1. Common subexpression elim
    // 2. Does not require formula to be in NNF
    // Does not implement:
    // 1. Polarity optimization (unclear if it helps)
    
    class TseitinTransformFull : public CNFConverterBase
    {
    private:
        vector<Literal> LiteralStack;
        typedef google::dense_hash_map<const ExpressionBase*, Literal> ExpLitMap;
        ExpLitMap ExpCache;

        // internal helper methods
        void TransformSubformula(const ExpressionBase* Exp,
                                 vector<Clause>& ClauseVec);
        inline void TransformVar(const ExpressionBase* Exp,
                                 vector<Clause>& ClauseVec);

        inline void TransformConjunction(u32 NumChildren, vector<Clause>& ClauseVec);
        inline void TransformNegatedConjunction(u32 NumChildrenn, vector<Clause>& ClauseVec);
        inline void TransformDisjunction(u32 NumChildrenn, vector<Clause>& ClauseVec);
        inline void TransformNegatedDisjunction(u32 NumChildrenn, vector<Clause>& ClauseVec);

        void TransformAnd(const ExpressionBase* Exp,
                          vector<Clause>& ClauseVec);
        void TransformOr(const ExpressionBase* Exp,
                         vector<Clause>& ClauseVec);
        void TransformXor(const ExpressionBase* Exp,
                          vector<Clause>& ClauseVec);
        void TransformXnor(const ExpressionBase* Exp,
                           vector<Clause>& ClauseVec);
        void TransformNand(const ExpressionBase* Exp,
                           vector<Clause>& ClauseVec);
        void TransformNor(const ExpressionBase* Exp,
                          vector<Clause>& ClauseVec);
        void TransformImplies(const ExpressionBase* Exp,
                              vector<Clause>& ClauseVec);
        void TransformIff(const ExpressionBase* Exp,
                          vector<Clause>& ClauseVec);
        

    public:
        TseitinTransformFull();
        virtual ~TseitinTransformFull();
        
        virtual void operator () (const ExpressionBase* Exp,
                                  vector<Clause>& ClauseVec) override;
        virtual void Reset() override;
    };

} /* end namespace */


#endif /* __SATLIB_TSEITIN_TRANSFORM_FULL_HPP */

