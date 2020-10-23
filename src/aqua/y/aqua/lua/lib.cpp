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

            // prepare lua
            lua_State *L = **vm;
            lua_settop(L,0);

            // get lib as table
            lua_getglobal(L,*name);
            if( !lua_istable(L,-1) )
            {
                throw exception("%s '%s' is <%s> instead of <table>",fn,*name,luaL_typename(L,-1));
            }

            // parse all items
            const size_t n = lua_rawlen(L,-1);
            for(size_t i=1;i<=n;++i)
            {
                const int t = lua_rawgeti(L,-1,i);
                std::cerr << "#item " << i << " is " << lua_typename(L,t) << std::endl;
                lua_pop(L,1);
            }


            // and initialize for immediate use
            lib.init();
        }

    }
}
