#if !defined __SATLIB_LINGELING_LOADER_HPP
#define __SATLIB_LINGELING_LOADER_HPP

#include "SatLibForwardDecls.hpp"
#include "PluginLoaderBase.hpp"
#include <dlfcn.h>

namespace SatLib {

    // This is a loader interface for the Lingeling class of SAT 
    // solvers. Lingeling solvers are written in C and it was too
    // much of a pain to port these to C++.
    // So we take the easy way out and simply load them into the
    // executable on demand

    // The macro SATLIB_PLUGIN_PATH MUST be defined on compilation!

    struct LGL;

    class LingelingLoader : PluginLoaderBase
    {
    private:
        // lingeling function handles
        // init/destroy
        LGL* (*Init)();
        void (*Release)(LGL*);

        // external iface
        i32 (*maxvar)(LGL*);
        i32 (*incvar)(LGL*);
        void (*add)(LGL*, i32);
        void (*assume)(LGL*, i32);
        void (*cassume)(LGL*, i32);
        void (*fixate) (LGL*);
        void (*meltall) (LGL*);
        i32 (*sat)(LGL*);
        i32 (*simp)(LGL*);
        i32 (*deref)(LGL*, i32);
        i32 (*fixed)(LGL*, i32);
        i32 (*failed)(LGL*, i32);
        i32 (*inconsistent)(LGL*);
        i32 (*changed)(LGL*);
        void (*flushcache)(LGL*);
        i32 (*repr)(LGL*, i32);

        LGL* SolverHandle;

    public:
        LingelingLoader(const string& LibName);
        virtual ~LingelingLoader();

        // The interface to lingeling
        i32 MaxVar();
        i32 IncVar();
        void Add(i32 Lit);
        void Assume(i32 Lit);
        void CAssume(i32 Lit);
        void Fixate();
        void MeltAll();
        i32 Sat();
        i32 Simp();
        i32 Deref(i32 Lit);
        i32 Fixed(i32 Lit);
        i32 Failed(i32 Lit);
        i32 Inconsistent();
        i32 Changed();
        void FlushCache();
        i32 Repr(i32);

        void Reset();
    };

} /* end namespace */


#endif /* __SATLIB_LINGELING_LOADER_HPP */

