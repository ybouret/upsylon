

#include "y/aqua/lua/interface.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon
{
    namespace Aqua
    {
        static const char fn[] = "Lua.Boot";

        static inline
        void __add(Boot          &boot,
                   const int      indx,
                   const string  &bootName,
                   const Library &lib,
                   lua_State     *L)
        {
            assert(lua_istable(L,-1));
            const int n = int(lua_rawlen(L,-1));
            if(n<2) throw exception("%s %s[%d] has not enough items",fn,*bootName,indx);

            // get constraint value
            double value = 0;
            {
                lua_rawgeti(L,-1,1);
                if(!lua_isnumber(L,-1))
                {
                    throw exception("%s %s[%d] constraint is not a number but <%s>",fn,*bootName,indx,luaL_typename(L,-1));
                }
                value = lua_tonumber(L,-1);
                lua_pop(L,1);
            }

            Constraint &cc = boot(value);
            {
                // parse { coef, name }
                for(int i=2,j=1;i<=n;++i,++j)
                {
                    const string ctx = vformat("%s %s[%d][%d]", fn, *bootName, indx, j);

                    lua_rawgeti(L,-1,i);
                    if(!lua_istable(L,-1)) throw exception("%s is not a <table> but <%s>", *ctx, luaL_typename(L,-1));

                    // get coef
                    int coef = 0;
                    {
                        lua_rawgeti(L,-1,1);
                        if(!lua_isinteger(L,-1)) throw exception("%s coefficient is not an <integer> but <%s>", *ctx, luaL_typename(L,-1));
                        coef = int(lua_tointeger(L,-1));
                        lua_pop(L,1);
                    }

                    // get name
                    string name;
                    {
                        lua_rawgeti(L,-1,2);
                        if(!lua_isstring(L,-1)) throw exception("%s coefficient is not a <string> but <%s>", *ctx, luaL_typename(L,-1));
                        name = lua_tostring(L,-1);
                        lua_pop(L,1);
                    }
                    lua_pop(L,1);

                    cc.add(lib[name],coef);
                }

            }



        }

        void __Lua:: Load(Boot          &boot,
                          const string  &bootName,
                          const Library &lib,
                          Lua::VM       &vm)
        {
            //------------------------------------------------------------------
            //
            // prepare lua
            //
            //------------------------------------------------------------------
            lua_State *L = **vm;
            lua_settop(L,0);
            // get lib as table
            lua_getglobal(L,*bootName);
            if( !lua_istable(L,-1) )
            {
                throw exception("%s %s is <%s> instead of <table>",fn,*bootName,luaL_typename(L,-1));
            }

            //------------------------------------------------------------------
            //
            // parse all items
            //
            //------------------------------------------------------------------
            const int n = int(lua_rawlen(L,-1));
            for(int indx=1;indx<=n;++indx)
            {
                const int t = lua_rawgeti(L,-1,indx);
                switch(t)
                {
                    case LUA_TTABLE:
                        __add(boot,indx,bootName,lib,L);
                        break;

                    case LUA_TSTRING: {
                        const string cmd = lua_tostring(L,-1);
                        if(cmd=="E/N")
                        {
                            boot.electroneutrality(lib);
                            break;
                        }
                        throw exception("%s %s[%d] unhandled command '%s' ",fn,*bootName,indx,*cmd);

                    } break;

                    default:
                        throw exception("%s %s[%d] unhandled <%s> ",fn,*bootName,indx,lua_typename(L,t));
                }

                lua_pop(L,1);
            }
        }


        void __Lua:: Load(Boot &boot, const char *name, const Library &lib, Lua::VM &vm )
        {
            const string _(name);
            Load(boot,_,lib,vm);
        }
    }

}


