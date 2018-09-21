
#include "y/lua++/function.hpp"

namespace upsylon
{

    namespace Lua
    {

        FunctionType::FunctionType( const string & __name, const VM & __vm) :
        name( __name ),
        vm(   __vm   )
        {
        }

        FunctionType:: ~FunctionType() throw()
        {
        }

        FunctionType:: FunctionType( const FunctionType &other ) :
        name( other.name ),
        vm( other.vm )
        {
        }

        void FunctionType:: pull()
        {
            lua_getglobal( **vm, *name);
        }

    }
}

