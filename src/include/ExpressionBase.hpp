#if !defined __SATLIB_EXPRESSION_BASE_HPP
#define __SATLIB_EXPRESSION_BASE_HPP

#include "SatLibForwardDecls.hpp"

namespace SatLib {

    class ExpressionPtrHash
    {
    public:
        u64 operator () (const ExpressionBase* Exp) const;
    };

    class ExpressionPtrEquals
    {
    public:
        bool operator () (const ExpressionBase* Exp1,
                          const ExpressionBase* Exp2) const;
    };

    class ExpressionBase
    {
    private:
        ExpressionManager* Mgr;
        ExpOperator Op;
        vector<const ExpressionBase*> Children;
        u64 VarID;
        u64 HashValue;
        bool Negated;

    public:
        // For proper expressions
        ExpressionBase(ExpressionManager* Mgr, ExpOperator Op, 
                       const vector<const ExpressionBase*>& Children,
                       bool Negated = false);
        // For variables
        ExpressionBase(ExpressionManager* Mgr, u64 VarID,
                       bool Negated = false);
        // For constants
        ExpressionBase(ExpressionManager* Mgr, bool ConstantValue);
        virtual ~ExpressionBase();
        
        // Accessors
        ExpressionManager* GetMgr() const;
        ExpOperator GetOp() const;
        const string& GetVarName() const;
        u64 GetVarID() const;
        const vector<const ExpressionBase*>& GetChildren() const;
        bool IsNegated() const;
        
        // Hash and comparison
        u64 Hash() const;
        bool operator == (const ExpressionBase& Other) const;

        // Visitors
        void Accept(ExpressionVisitorBase* Visitor) const;

        // Stringification
        string ToString() const;
    };

    ostream& operator << (ostream& os, const ExpressionBase& Exp);

} /* End namespace */

#endif /* __SATLIB_EXPRESSION_BASE_HPP */

