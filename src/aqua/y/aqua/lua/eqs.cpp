
#include "y/aqua/lua/interface.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon
{
    namespace Aqua
    {



        static const char fn[] = "Lua.Equilibria";

        static inline
        void __add(lua_State     *L,
                   Equilibrium   &eq,
                   const char    *ctx,
                   const Library &lib)
        {
            if(!lua_istable(L,-1)) throw exception("%s is not a table",ctx);


            // get nu
            int nu = 0;
            {
                lua_rawgeti(L,-1,1);
                if( !lua_isinteger(L,-1))  throw exception("%s coefficient is not <integer> but <%s>", ctx, luaL_typename(L,-1));
                nu = int(lua_tointeger(L,-1));
                lua_pop(L,1);
            }
            
            // get id
            string id;
            {
                lua_rawgeti(L,-1,2);
                if( !lua_isstring(L,-1) ) throw exception("%s identifier it not <string> but <%s>", ctx, luaL_typename(L,-1) );
                id = lua_tostring(L,-1);
                lua_pop(L,1);
            }
            eq(lib[id],nu);
        }

        static inline
        void __add(Equilibria    &eqs,
                   const char    *eqsName,
                   lua_State     *L,
                   const int      indx,
                   const Library &lib)
        {
            assert( LUA_TTABLE == lua_type(L,-1) );

            const int n = int(lua_rawlen(L,-1));
            if(n<3) throw exception("%s %s[%d] has not enough items (found %d)", fn, eqsName, indx, n);


            //------------------------------------------------------------------
            //
            // get equilibrium name
            //
            //------------------------------------------------------------------
            string name;
            {
                lua_rawgeti(L,-1,1);
                if(!lua_isstring(L,-1)) throw exception("%s %s[%d][1] is no <string> but <%s>",fn,eqsName,indx,luaL_typename(L,-1));
                name = lua_tostring(L,-1);
                lua_pop(L,1);
            }
            const char *eqName = *name;

            //------------------------------------------------------------------
            //
            // get equilibrium constant/type
            //
            //------------------------------------------------------------------
            auto_ptr<Equilibrium> pEq = 0;
            {
                lua_rawgeti(L,-1,2);
                switch(lua_type(L,-1))
                {
                    case LUA_TNUMBER:
                        pEq = new ConstEquilibrium(name,lua_tonumber(L,-1));
                        break;

                    default:
                        throw exception("%s %s.%s constant can't be <%s>",fn,eqsName,eqName,luaL_typename(L,-1));
                }
                lua_pop(L,1);
            }

            //------------------------------------------------------------------
            //
            // get equilibrium constant/type
            //
            //------------------------------------------------------------------
            for(int i=3,j=1;i<=n;++i,++j)
            {
                lua_rawgeti(L,-1,i);
                const string cname = vformat("%s %s.%s component[%d]",fn,eqsName,eqName,j);
                // parse { nu, name }
                {
                    __add(L,*pEq,*cname,lib);
                }

                lua_pop(L,1);
            }

            //------------------------------------------------------------------
            //
            // insert into database
            //
            //------------------------------------------------------------------
            pEq->validate();
            (void) eqs( pEq.yield() );


        }

        void __Lua:: Load(Equilibria    &eqs,
                          const string  &name,
                          const Library &lib,
                          Lua::VM       &vm )
        {

            //------------------------------------------------------------------
            //
            // prepare lua
            //
            //------------------------------------------------------------------
            lua_State *L = **vm;
            lua_settop(L,0);
            const char *eqsName = *name;
            // get lib as table
            lua_getglobal(L,eqsName);
            if( !lua_istable(L,-1) )
            {
                throw exception("%s %s is <%s> instead of <table>",fn,eqsName,luaL_typename(L,-1));
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
                if( LUA_TTABLE != t)
                {
                    throw exception("%s %s[%d] is not <table> but <%s>",fn,eqsName,indx,lua_typename(L,t));
                }
                __add(eqs,eqsName,L,indx,lib);
                lua_pop(L,1);
            }

        }

        void __Lua:: Load(Equilibria    &eqs,
                          const char    *name,
                          const Library &lib,
                          Lua::VM       &vm)
        {
            const string _(name);
            Load(eqs,_,lib,vm);
        }

    }
}
