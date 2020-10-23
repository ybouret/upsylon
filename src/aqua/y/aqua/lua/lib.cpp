#include "y/aqua/lua/interface.hpp"


namespace upsylon
{
    namespace Aqua
    {

        static const char fn[] = "__Lua.Load.Library";

        static inline
        void __add_species(Library    &lib,
                           const char *libName,
                           lua_State  *L,
                           const int   indx)
        {
            assert( LUA_TTABLE == lua_type(L,-1) );

            const int n = lua_rawlen(L,-1);
            if(n<2) throw exception("%s %s[%d] has not enough items (found %d)", fn, libName, indx, n);

            // get species name
            string name;
            {
                lua_rawgeti(L,-1,1);
                if(!lua_isstring(L,-1)) throw exception("%s %s[%d][1] is no <string> but <%s>",fn,libName,indx,luaL_typename(L,-1));
                name = lua_tostring(L,-1);
                lua_pop(L,1);
            }

            // get species charge
            int Z=0;
            {
                lua_rawgeti(L,-1,2);
                if(!lua_isinteger(L,-1)) throw exception("%s %s[%d][2] is no <integer> but <%s>",fn,libName,indx,luaL_typename(L,-1));
                Z = lua_tointeger(L,-1);
                lua_pop(L,1);
            }

            lib(name,Z);

        }

        void __Lua:: Load(Library      &lib,
                          const string &name,
                          Lua::VM      &vm )
        {

            // prepare lua
            lua_State *L = **vm;
            lua_settop(L,0);
            const char *libName = *name;
            // get lib as table
            lua_getglobal(L,libName);
            if( !lua_istable(L,-1) )
            {
                throw exception("%s %s is <%s> instead of <table>",fn,*name,luaL_typename(L,-1));
            }

            // parse all items
            const int n = lua_rawlen(L,-1);
            for(int indx=1;indx<=n;++indx)
            {
                const int t = lua_rawgeti(L,-1,indx);
                if( LUA_TTABLE != t)
                {
                    throw exception("%s %s[%d] is not <table> but <%s>",fn,libName,indx,lua_typename(L,t));
                }
                __add_species(lib,libName,L,indx);
                lua_pop(L,1);
            }


            // and initialize for immediate use
            lib.init();

            std::cerr << lib << std::endl;
        }

    }
}
