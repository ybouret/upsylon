
#include "y/alchemy/equilibria.hpp"
#include "y/alchemy/library.hpp"
#include "y/string/tokenizer.hpp"
#include "y/sequence/list.hpp"

namespace upsylon
{

    namespace Alchemy
    {
        void Equilibria:: load(const string &id,
                               Library      &lib,
                               Lua::VM      &vm)
        {
            static const char fn[] = ".load";

            lua_State *L = **vm;
            lua_settop(L,0);
            lua_getglobal(L,*id);
            if(!lua_istable(L,-1)) throw exception("%s%s '%s' is not a table",CLID,fn,*id);
            const int n = int( lua_rawlen(L,-1) );
            for(int i=1;i<=n;++i)
            {
                lua_rawgeti(L,-1,i);
                if(!lua_isstring(L,-1) )throw exception("%s%s %s[%d] is not a string",CLID,fn,*id,i);
                const string info = lua_tostring(L,-1);
                lua_pop(L,1);
                parse(info,lib,vm);
            }

        }

    }

}

