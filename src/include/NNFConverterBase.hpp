#if !defined __SATLIB_NNF_CONVERTER_BASE_HPP
#define __SATLIB_NNF_CONVERTER_BASE_HPP

#include "SatLibForwardDecls.hpp"

namespace SatLib {

    class NNFConverterBase
    {
    private:
        string ConverterName;

    public:
        NNFConverterBase(const string& ConverterName);
        virtual ~NNFConverterBase();
        
        virtual const ExpressionBase* operator () (ExpressionManager* Mgr, 
                                                   const ExpressionBase* Exp) = 0;
        const string& GetName() const;
    };

} /* End namespace */

#endif /* __SATLIB_NNF_CONVERTER_BASE_HPP */

