
#include "y/lua++/function.hpp"

namespace upsylon
{

    namespace Lua
    {

        FunctionType::FunctionType( const string & __name, const VM & __vm) :
        name( __name ),
        vm(   __vm   ),
        L( **vm )
        {
        }

        FunctionType:: ~FunctionType() throw()
        {
        }

        FunctionType:: FunctionType( const FunctionType &other ) :
        name( other.name ),
        vm( other.vm ),
        L( **vm )
        {
        }

        void FunctionType:: pull()
        {
            assert(L);
            //std::cerr << "lua_getglobal(" << *name << ")" << std::endl;
            lua_getglobal(L, *name);
            //std::cerr << luaL_typename(L,-1) << std::endl;
            //assert(lua_isfunction(L,-1));
        }

        void FunctionType:: call( const int narg, const int nret)
        {
            if( lua_pcall(L,narg,nret,0) )
            {
                throw exception("Lua::Function::call '%s' : %s", *name, lua_tostring(L,-1));
            }
        }


    }
}

