#if !defined __SATLIB_SATLIB_LINGELING_SOLVER_HPP
#define __SATLIB_SATLIB_LINGELING_SOLVER_HPP

#include "SatLibForwardDecls.hpp"
#include "SatLibSolverBase.hpp"
#include "LingelingLoader.hpp"
#include <unordered_map>

namespace SatLib {

    class SatLibLingelingSolver : public SatLibSolverBase
    {
    private:
        LingelingLoader* TheSolver;
        u32 LastSolveNumAssertions;
        unordered_map<u32, i32> VMap;
        SolveResult LastSolveResult;

    public:
        SatLibLingelingSolver(const string& SolverPluginName);
        virtual ~SatLibLingelingSolver();
        
        virtual SolveResult Solve() override;
        virtual void GetModel(SatModel& Model) override;
        virtual void Push() override;
        virtual void Pop(u32 NumScopes = 1) override;
    };

} /* end namespace */

#endif /* __SATLIB_SATLIB_LINGELING_SOLVER_HPP */

