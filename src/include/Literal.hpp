#if !defined __SATLIB_LITERAL_HPP
#define __SATLIB_LITERAL_HPP

#include "SatLibForwardDecls.hpp"
#include <boost/functional/hash.hpp>

namespace SatLib {


    class Literal
    {
    private:
        u32 VarID;
        bool Negated;

    public:
        Literal();
        Literal(u32 VarID, bool Negated = false);
        Literal(const Literal& Other);
        ~Literal();
        // Accessors
        u32 GetVarID() const;
        bool IsNegated() const;
        bool IsAux() const;
        Literal Negate() const;
        string ToString() const;

        Literal& operator = (const Literal& Other);
        bool operator == (const Literal& Other) const;

        inline u64 Hash() const
        {
            u64 Retval = 0;
            boost::hash_combine(Retval, (u64)VarID);
            boost::hash_combine(Retval, (u64)Negated);
            return Retval;
        }
    };

    class LiteralHash
    {
    public:
        inline u64 operator () (const Literal& Lit) const
        {
            return Lit.Hash();
        }
    };

    ostream& operator << (ostream& str, const Literal& Lit);

} /* End namespace */

#endif /* __SATLIB_LITERAL_HPP */

