

#include "y/chem/lua/io.hpp"

namespace upsylon
{
    namespace Chemical
    {

        static inline
        void __load_equilbrium( Equilibria &eqs, Lua::State::Pointer &vm, const char *id, const Library &lib, const int count )
        {
            lua_State *L = **vm;
            assert(lua_istable(L,-1));
            
        }

        void __luaIO:: add( Equilibria &eqs, Lua::State::Pointer &vm, const string &name, const Library &lib )
        {
            lua_State *L = **vm;

            lua_settop(L,0);
            lua_getglobal(L,*name);
            if(!lua_istable(L,-1))
            {
                throw exception("no equilibria table '%s'", *name);
            }

            //__________________________________________________________________
            //
            // loop on table items = eq
            //__________________________________________________________________
            // stack now contains: -1 => table
            lua_pushnil(L);
            // stack now contains: -1 => nil; -2 => table
            int count = 0;
            while( lua_next(L,-2) )
            {
                // stack now contains: -1 => value; -2 => key; -3 => table
                ++count;
                __load_equilbrium(eqs,vm, *name, lib, count);
                // remove value
                lua_pop(L,1);
            }
            // nil
            lua_pop(L,1);
        }

    }

}


