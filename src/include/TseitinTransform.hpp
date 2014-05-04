#if !defined __SATLIB_TSEITIN_TRANSFORM_HPP
#define __SATLIB_TSEITIN_TRANSFORM_HPP

#include "SatLibForwardDecls.hpp"
#include "CNFConverterBase.hpp"

namespace SatLib {

    class TseitinTransform : public CNFConverterBase
    {
    private:
        vector<Literal> LiteralStack;
        
        void TransformSubformula(const ExpressionBase* Exp,
                                 vector<Clause>& ClauseVec);
        inline void TransformVar(const ExpressionBase* Exp,
                                 vector<Clause>& ClauseVec);
        inline void TransformAnd(const ExpressionBase* Exp,
                                 vector<Clause>& ClauseVec);
        inline void TransformOr(const ExpressionBase* Exp,
                                vector<Clause>& ClauseVec);

    public:
        TseitinTransform();
        virtual ~TseitinTransform();
        
        virtual void operator () (const ExpressionBase* Exp,
                                  vector<Clause>& ClauseVec);

        virtual void Reset() override;
    };

} /* End namespace */

#endif /* __SATLIB_TSEITIN_TRANSFORM_HPP */

