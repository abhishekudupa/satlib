#if !defined __SATLIB_UTILS_HPP
#define __SATLIB_UTILS_HPP

#include "SatLibCommon.hpp"
#include "SatLibForwardDecls.hpp"
#include "Literal.hpp"

namespace SatLib {

    static inline const ExpressionBase* TryMigrate(ExpressionManager* Mgr, 
                                                   const ExpressionBase* Exp)
    {
        if(Exp->GetMgr() != Mgr) {
            return Mgr->Migrate(Exp);
        } else {
            return Exp;
        }
    }

    static inline void TryMigrate(ExpressionManager* Mgr,
                                  const vector<const ExpressionBase*>& Exps,
                                  vector<const ExpressionBase*>& Migs)
    {
        const u32 NumChildren = Exps.size();
        for(u32 i = 0; i < NumChildren; ++i) {
            Migs.push_back(TryMigrate(Mgr, Exps[i]));
        }
    }

    static inline void PrintClauses(ostream& str, const vector<Clause>& ClauseVec)
    {
        const u32 NumClauses = ClauseVec.size();
        for(u32 i = 0; i < NumClauses; ++i) {
            str << "(";
            const u32 NumLits = ClauseVec[i].size();
            for(u32 j = 0; j < NumLits; ++j) {
                str << ClauseVec[i][j];
                if(j != NumLits - 1) {
                    str << " | ";
                }
            }
            str << ")";
            if(i != NumClauses - 1) {
                str << "," << endl;
            }
        }
        str << endl;
    }

    static inline string ClauseVecToString(const vector<Clause>& ClauseVec)
    {
        ostringstream sstr;
        PrintClauses(sstr, ClauseVec);
        return sstr.str();
    }


} /* End namespace */

#endif /* __SATLIB_UTILS_HPP */

