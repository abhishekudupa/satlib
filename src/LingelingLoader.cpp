#include <LingelingLoader.hpp>
#include <SatLibExceptions.hpp>

namespace SatLib {

    LingelingLoader::LingelingLoader(const string& LibName)
        : PluginLoaderBase(LibName),
          Init((LGL* (*)())ResolveSymbol("lglinit")),
          Release((void (*)(LGL*))ResolveSymbol("lglrelease")),
          maxvar((i32 (*)(LGL*))ResolveSymbol("lglmaxvar")),
          incvar((i32 (*)(LGL*))ResolveSymbol("lglincvar")),
          add((void (*)(LGL*, i32))ResolveSymbol("lgladd")),
          assume((void (*)(LGL*, i32))ResolveSymbol("lglassume")),
          cassume((void (*)(LGL*, i32))ResolveSymbol("lglcassume")),
          fixate((void (*)(LGL*))ResolveSymbol("lglfixate")),
          meltall((void (*)(LGL*))ResolveSymbol("lglmeltall")),
          sat((i32 (*)(LGL*))ResolveSymbol("lglsat")),
          simp((i32 (*)(LGL*))ResolveSymbol("lglsimp")),
          deref((i32 (*)(LGL*, i32))ResolveSymbol("lglderef")),
          fixed((i32 (*)(LGL*, i32))ResolveSymbol("lglfixed")),
          failed((i32 (*)(LGL*, i32))ResolveSymbol("lglfailed")),
          inconsistent((i32 (*)(LGL*))ResolveSymbol("lglinconsistent")),
          changed((i32 (*)(LGL*))ResolveSymbol("lglchanged")),
          flushcache((void (*)(LGL*))ResolveSymbol("lglflushcache")),
          repr((i32 (*)(LGL*, i32))ResolveSymbol("lglrepr")),
          SolverHandle(Init())
    {
        if (SolverHandle == NULL) {
            throw SolverException("Error: LGLSolver: Could not initialize");
        }
    }

    LingelingLoader::~LingelingLoader()
    {
        Release(SolverHandle);
    }

    i32 LingelingLoader::MaxVar()
    {
        return maxvar(SolverHandle);
    }

    i32 LingelingLoader::IncVar()
    {
        return incvar(SolverHandle);
    }

    void LingelingLoader::Add(i32 Lit)
    {
        add(SolverHandle, Lit);
    }

    void LingelingLoader::Assume(i32 Lit)
    {
        assume(SolverHandle, Lit);
    }
    
    void LingelingLoader::CAssume(i32 Lit)
    {
        cassume(SolverHandle, Lit);
    }

    void LingelingLoader::Fixate()
    {
        fixate(SolverHandle);
    }

    void LingelingLoader::MeltAll()
    {
        meltall(SolverHandle);
    }

    i32 LingelingLoader::Sat()
    {
        return sat(SolverHandle);
    }

    i32 LingelingLoader::Simp()
    {
        return simp(SolverHandle);
    }

    i32 LingelingLoader::Deref(i32 Lit)
    {
        return deref(SolverHandle, Lit);
    }

    i32 LingelingLoader::Fixed(i32 Lit)
    {
        return fixed(SolverHandle, Lit);
    }

    i32 LingelingLoader::Failed(i32 Lit)
    {
        return failed(SolverHandle, Lit);
    }

    i32 LingelingLoader::Inconsistent()
    {
        return inconsistent(SolverHandle);
    }

    i32 LingelingLoader::Changed()
    {
        return changed(SolverHandle);
    }

    void LingelingLoader::FlushCache()
    {
        flushcache(SolverHandle);
    }

    i32 LingelingLoader::Repr(i32 Lit)
    {
        return repr(SolverHandle, Lit);
    }

    void LingelingLoader::Reset()
    {
        Release(SolverHandle);
        SolverHandle = Init();
    }

} /* end namespace */

