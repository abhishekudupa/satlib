#include <PluginLoaderBase.hpp>
#include <dlfcn.h>
#include <SatLibExceptions.hpp>

namespace SatLib {

    PluginLoaderBase::PluginLoaderBase(const string& Path)
    {
        auto LibPath = (string)SATLIB_PLUGIN_PATH + "/" + Path;
        Handle = dlopen(LibPath.c_str(), RTLD_LAZY);
        if (Handle == NULL) {
            throw PluginLoaderException((string)"Could not load library: " + LibPath);
        }        
    }

    PluginLoaderBase::~PluginLoaderBase()
    {
        if (dlclose(Handle) != 0) {
            throw PluginLoaderException("Error when closing library!");
        }
    }

    void* PluginLoaderBase::ResolveSymbol(const string& SymbolName)
    {
        // throw away earlier errors
        (void)dlerror();
        auto Retval = dlsym(Handle, SymbolName.c_str());
        auto ErrStat = dlerror();
        if (ErrStat != NULL) {
            throw PluginLoaderException((string)"Error resolving symbol: " + SymbolName + ":\n" +
                                        ErrStat);
        }
        return Retval;
    }

} /* end namespace */

