//! \file

#ifndef Y_AQUA_LUA_INCLUDED
#define Y_AQUA_LUA_INCLUDED 1

#include "y/aqua/library.hpp"
#include "y/lua++/state.hpp"

namespace upsylon
{
    namespace Aqua
    {
        struct __Lua
        {
            static void Load(Library &lib, const string &name, Lua::VM &vm );
        };
    }
}


#endif
