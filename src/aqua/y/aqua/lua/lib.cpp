#include "y/aqua/lua/interface.hpp"


namespace upsylon
{
    namespace Aqua
    {
        void __Lua:: Load(Library      &lib,
                          const string &name,
                          Lua::VM      &vm )
        {
            static const char fn[] = "__Lua.Load.Library";
            lua_State *L = **vm;
            lua_settop(L,0);
            lua_getglobal(L,*name);
            if( !lua_istable(L,-1) )
            {
                throw exception("%s '%s' is <%s> instead of <table>",fn,*name,luaL_typename(L,-1));
            }

            lib.init();
        }

    }
}
