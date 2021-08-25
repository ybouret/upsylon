

#include "y/alchemy/library.hpp"
#include "y/exception.hpp"
#include "y/lua++/state.hpp"

namespace upsylon
{
    namespace Alchemy
    {
        void Library:: load(const string &id, Lua::VM &vm)
        {
            static const char fn[] = ".load";
            lua_State *L = **vm;
            lua_settop(L,0);
            lua_getglobal(L,*id);
            if( ! lua_istable(L,-1) ) throw exception("%s%s '%s' is not a table",CLID,fn,*id);
            const unsigned long n = lua_rawlen(L,-1);
            for(unsigned long i=1;i<=n;++i)
            {
                lua_rawgeti(L,-1,i);
                if(!lua_isstring(L,-1)) throw exception("%s%s %s[%lu] is not a string",CLID,fn,*id,i);
                get( lua_tostring(L,-1) );
                lua_pop(L,1);
            }
        }

    }

}

