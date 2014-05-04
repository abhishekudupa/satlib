#include <Literal.hpp>
#include <SatLibExceptions.hpp>
#include <iomanip>

namespace SatLib {

    Literal::Literal()
        : VarID(0), Negated(false)
    {
        // Nothing here
    }

    Literal::Literal(u32 VarID, bool Negated)
        : VarID(VarID), Negated(Negated)
    {
        // Nothing here
    }
    
    Literal::Literal(const Literal& Other)
        : VarID(Other.VarID), Negated(Other.Negated)
    {
        // Nothing here
    }

    Literal::~Literal()
    {
        // Nothing here
    }

    Literal& Literal::operator = (const Literal& Other)
    {
        if (&Other == this) {
            return *this;
        }
        VarID = Other.VarID;
        Negated = Other.Negated;
        return *this;
    }

    bool Literal::operator == (const Literal& Other) const
    {
        return (Other.VarID == VarID && Other.Negated == Negated);
    }

    u32 Literal::GetVarID() const
    {
        return VarID;
    }

    bool Literal::IsNegated() const
    {
        return Negated;
    }

    Literal Literal::Negate() const
    {
        return Literal(VarID, !Negated);
    }

    string Literal::ToString() const
    {
        ostringstream sstr;
        sstr << *this;
        return sstr.str();
    }

    bool Literal::IsAux() const
    {
        return (VarID >= SATLIB_AUX_VAR_OFFSET); 
    }

    ostream& operator << (ostream& str, const Literal& Lit)
    {
        if(Lit.IsNegated()) {
            str << "!";
        }
        str << "Lit_" << setw(8) << setfill('0') << Lit.GetVarID();
        return str;
    }

} /* End namespace */
