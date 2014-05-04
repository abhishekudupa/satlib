#if !defined __SATLIB_SIMPLE_NNF_CONVERTER_HPP
#define __SATLIB_SIMPLE_NNF_CONVERTER_HPP

#include "SatLibForwardDecls.hpp"
#include "NNFConverterBase.hpp"

namespace SatLib {

    class SimpleNNFConverter : public NNFConverterBase
    {
    private:
        // Utility functions
        const ExpressionBase* Convert(ExpressionManager* Mgr, const ExpressionBase* Exp);

        // Normalization functions
        inline const ExpressionBase* NormalizeXOR(ExpressionManager* Mgr, const ExpressionBase* Exp);
        inline const ExpressionBase* NormalizeXNOR(ExpressionManager* Mgr, const ExpressionBase* Exp);
        inline const ExpressionBase* NormalizeNAND(ExpressionManager* Mgr, const ExpressionBase* Exp);
        inline const ExpressionBase* NormalizeNOR(ExpressionManager* Mgr, const ExpressionBase* Exp);
        inline const ExpressionBase* NormalizeImplies(ExpressionManager* Mgr, const ExpressionBase* Exp);
        inline const ExpressionBase* NormalizeIff(ExpressionManager* Mgr, const ExpressionBase* Exp);
        inline const ExpressionBase* PushNegations(ExpressionManager* Mgr, const ExpressionBase* Exp);
        inline const ExpressionBase* NormalizeExp(ExpressionManager* Mgr, const ExpressionBase* Exp);

    public:
        SimpleNNFConverter();
        virtual ~SimpleNNFConverter();
        
        virtual const ExpressionBase* operator () (ExpressionManager* Mgr, const ExpressionBase* Exp);
    };

} /* End namespace */

#endif /* __SATLIB_SIMPLE_NNF_CONVERTER_HPP */

