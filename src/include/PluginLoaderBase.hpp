#if !defined __SATLIB_PLUGIN_LOADER_BASE_HPP
#define __SATLIB_PLUGIN_LOADER_BASE_HPP

#include "SatLibForwardDecls.hpp"
#include "SatLibCommon.hpp"

namespace SatLib {

    // Base for all plugin loaders
    // The macro SATLIB_PLUGIN_PATH must be defined on compilation

    class PluginLoaderBase
    {
    protected:
        void *Handle;
        void* ResolveSymbol(const string& SymbolName);

    public:
        PluginLoaderBase(const string& Path);
        virtual ~PluginLoaderBase();
    };

} /* end namespace */

#endif /* __SATLIB_PLUGIN_LOADER_BASE_HPP */

