#include <ExpressionBase.hpp>
#include <SatLibExceptions.hpp>
#include <ExpressionVisitorBase.hpp>
#include <ExpressionPrinter.hpp>
#include <algorithm>
#include <boost/functional/hash.hpp>
#include <ExpressionManager.hpp>
#include <iosfwd>

namespace SatLib {

    // Hash and comparator implementation
    u64 ExpressionPtrHash::operator () (const ExpressionBase* Exp) const
    {
        return Exp->Hash();
    }

    bool ExpressionPtrEquals::operator () (const ExpressionBase* Exp1,
                                           const ExpressionBase* Exp2) const
    {
        if (Exp1 == NULL && Exp2 == NULL) {
            return true;
        } else if (Exp1 == NULL || Exp2 == NULL) {
            return false;
        } else {
            return ((*Exp1) == (*Exp2));
        }
    }

    ExpressionBase::ExpressionBase(ExpressionManager* Mgr, ExpOperator Op,
                                   const vector<const ExpressionBase*>& Children,
                                   bool Negated)
        : Mgr(Mgr), Op(Op), Children(Children), VarID(0), Negated(Negated)
    {
        switch(Op) {
        case EXP_VAR:
            throw InvalidExpressionException("VAR Expressions cannot have children!");

        default:
            if (Children.size() < 2) {
                throw InvalidExpressionException("At least two children needed for expression");
            }
        }

        // Sort the children
        if (Children.size() != 2) {
            sort(this->Children.begin(), this->Children.end());
        }
        // compute the hash value
        HashValue = (u64)0;
        boost::hash_combine(HashValue, (u64)Op);
        for (auto const& Child : this->Children) {
            boost::hash_combine(HashValue, Child->Hash());
        }
        boost::hash_combine(HashValue, (u64)Negated);
    }

    ExpressionBase::ExpressionBase(ExpressionManager* Mgr, u64 VarID, bool Negated)
        : Mgr(Mgr), Op(EXP_VAR), VarID(VarID), Negated(Negated)
    {
        HashValue = (u64)0;
        boost::hash_combine(HashValue, (u64)Op);
        boost::hash_combine(HashValue, VarID);
        boost::hash_combine(HashValue, (u64)Negated);
    }

    ExpressionBase::ExpressionBase(ExpressionManager* Mgr, bool ConstantValue)
        : Mgr(Mgr), Op(ConstantValue ? EXP_TRUE : EXP_FALSE), VarID(0), Negated(false)
    {
        HashValue = (u64)0;
        boost::hash_combine(HashValue, (u64)Op);
    }

    ExpressionBase::~ExpressionBase()
    {
        // Nothing here
    }

    ExpressionManager* ExpressionBase::GetMgr() const
    {
        return Mgr;
    }

    ExpOperator ExpressionBase::GetOp() const
    {
        return Op;
    }

    const string& ExpressionBase::GetVarName() const
    {
        if(Op != EXP_VAR) {
            throw InvalidExpressionException("Tried to access VarName of non-variable expression");
        }

        // dispatch to the manager
        return Mgr->GetVarName(VarID);
    }

    u64 ExpressionBase::GetVarID() const
    {
        if(Op != EXP_VAR) {
            throw InvalidExpressionException("Tried to access VarID of non-variable expression");
        }
        return VarID;
    }

    const vector<const ExpressionBase*>& ExpressionBase::GetChildren() const
    {
        if(Op == EXP_VAR || Op == EXP_FALSE || Op == EXP_TRUE) {
            throw InvalidExpressionException("Tried to access Children of variable expression");
        }
        return Children;
    }

    bool ExpressionBase::IsNegated() const
    {
        return Negated;
    }

    void ExpressionBase::Accept(ExpressionVisitorBase* Visitor) const
    {
        Visitor->PreVisit(this);
        const u32 NumChildren = Children.size();
        if(NumChildren == 0) {
            Visitor->Visit(this, 0);
        }
        for(u32 i = 0; i < NumChildren; ++i) {
            Children[i]->Accept(Visitor);
            Visitor->Visit(this, i);
        }

        Visitor->PostVisit(this);
    }

    u64 ExpressionBase::Hash() const
    {
        return HashValue;
    }

    bool ExpressionBase::operator == (const ExpressionBase& Other) const
    {
        if (Op != Other.Op) {
            return false;
        }

        switch (Op) {
        case EXP_VAR:
            return (VarID == Other.VarID && Negated == Other.Negated);
        case EXP_TRUE:
        case EXP_FALSE:
            return true;
        default:
            if (Children.size() != Other.Children.size()) {
                return false;
            }

            for(u32 i = 0; i < Children.size(); ++i) {
                if (Children[i] != Other.Children[i]) {
                    return false;
                }
            }
            return (Negated == Other.Negated);
        }
    }

    string ExpressionBase::ToString() const
    {
        ostringstream sstr;
        sstr << *this;
        return sstr.str();
    }

    ostream& operator << (ostream& os, const ExpressionBase& Exp)
    {
        ExpressionPrinter Printer(os);
        Exp.Accept(&Printer);
        return os;
    }

} /* End namespace */

