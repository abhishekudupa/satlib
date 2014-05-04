#if !defined __SATLIB_CNF_CONVERTER_BASE_HPP
#define __SATLIB_CNF_CONVERTER_BASE_HPP

#include "SatLibForwardDecls.hpp"

namespace SatLib {

    /* Abstract Functor that represents a CNF converter */
    class CNFConverterBase
    {
    private:
        string ConverterName;

    public:
        CNFConverterBase(const string& ConverterName);
        virtual ~CNFConverterBase();
        
        virtual void operator () (const ExpressionBase* Exp,
                                  vector<Clause>& ClauseVec) = 0;
        const string& GetName() const;
        virtual void Reset() = 0;
    };

} /* End namespace */

#endif /* __SATLIB_CNF_CONVERTER_BASE_HPP */

