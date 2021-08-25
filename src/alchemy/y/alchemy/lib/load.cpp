

#include "y/alchemy/library.hpp"
#include "y/exception.hpp"
#include "y/lua++/state.hpp"

namespace upsylon
{
    namespace Alchemy
    {
        void Library:: load(const string &id, Lua::State &vm)
        {
            static const char fn[] = ".load";
            lua_State *L = *vm;
            lua_settop(L,0);
            lua_getglobal(L,*id);
            if( ! lua_istable(L,-1) ) throw exception("%s%s '%s' is not a table",CLID,fn,*id);
        }

    }

}

