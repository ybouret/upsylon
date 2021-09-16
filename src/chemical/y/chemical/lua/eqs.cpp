
#include "y/chemical/lua/eqs.hpp"

namespace upsylon
{
    namespace Chemical
    {
        LuaEquilibria:: ~LuaEquilibria() throw()
        {
        }

        LuaEquilibria:: LuaEquilibria(Library &usr,Lua::VM &_vm) :
        Equilibria(),
        lib(usr),
        lvm(_vm)
        {
        }

        void LuaEquilibria:: append(const string &tableName)
        {
            static const char fn[] = "::append: ";

            // initialize
            lua_State     *L    = **lvm;

            // get table
            lua_settop(L,0);
            lua_getglobal(L,*tableName);
            if( !lua_istable(L,-1) ) throw exception("%s%s '%s' is not a LUA table", CLID,fn, *tableName);

            // loop over table items, must be strings
            const long n = lua_rawlen(L,-1);
            std::cerr << "#" << tableName << " = " << n << std::endl;

            Strings info;

            for(long i=1;i<=n;++i)
            {
                assert(lua_istable(L,-1));
                lua_rawgeti(L,-1,i);
                if(!lua_isstring(L,-1)) throw exception("%s%s %s[%ld] is not a string",CLID,fn,*tableName,i);
                const string s = lua_tostring(L,-1);
                info << s;
                lua_pop(L,1);
            }

            for(size_t i=1;i<=info.size();++i)
            {
                (*this) << info[i];
            }
        }

    }

}


