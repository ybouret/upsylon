//! \file
#ifndef Y_CHEM_LUA_IO_INCLUDED
#define Y_CHEM_LUA_IO_INCLUDED 1

#include "y/chem/equilibria.hpp"
#include "y/lua++/state.hpp"

namespace upsylon
{
    namespace Chemical
    {

        //! I/O function to setup chemical systems with lua
        struct __luaIO
        {
            //! append species in table 'name' to lib
            static void add( Library &lib, Lua::State::Pointer &vm, const string &name );

            //! append equilibria from table 'name' to eqs, using species from lib
            static void add( Equilibria &eqs, Lua::State::Pointer &vm, const string &name, const Library &lib );

            
            //! append constraints from table 'name' to eqs, using species from lib
            static void add( Boot::Loader &loader, Lua::State::Pointer &vm, const string &name, Library &lib );

            //! inline boot, assuming constants are computed
            static inline
            bool boot( array<double> &C, Lua::State::Pointer &vm, const string &name, Equilibria &eqs, Library &lib )
            {
                Boot::Loader ld;
                add(ld,vm,name,lib);
                return eqs.boot(C,ld);
            }

        };

    }
}


#endif

