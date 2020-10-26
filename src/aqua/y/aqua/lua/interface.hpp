//! \file

#ifndef Y_AQUA_LUA_INCLUDED
#define Y_AQUA_LUA_INCLUDED 1

#include "y/aqua/library.hpp"
#include "y/aqua/equilibria.hpp"
#include "y/aqua/boot.hpp"
#include "y/lua++/state.hpp"

namespace upsylon
{
    namespace Aqua
    {
        struct __Lua
        {
            static void Load(Library &lib, const string &name, Lua::VM &vm );
            static void Load(Library &lib, const char   *name, Lua::VM &vm );

            static void Load(Equilibria &eqs, const string &name,  const Library &lib, Lua::VM &vm );
            static void Load(Equilibria &eqs, const char   *name,  const Library &lib, Lua::VM &vm );

            static void Load(Boot &boot, const string & name, const Library &lib, Lua::VM &vm );
            static void Load(Boot &boot, const char   * name, const Library &lib, Lua::VM &vm );

        };
    }
}


#endif
