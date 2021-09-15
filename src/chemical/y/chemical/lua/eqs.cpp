
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


    }

}


