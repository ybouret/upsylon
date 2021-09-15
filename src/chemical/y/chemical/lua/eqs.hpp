

//! \file

#ifndef Y_CHEMICAL_LUA_EQUILIBRIA_INCLUDED
#define Y_CHEMICAL_LUA_EQUILIBRIA_INCLUDED 1

#include "y/chemical/equilibria.hpp"
#include "y/lua++/state.hpp"

namespace upsylon
{
    namespace Chemical
    {

        class LuaEquilibria : public Equilibria
        {
        public:
            explicit LuaEquilibria(Library &, Lua::VM &);
            virtual ~LuaEquilibria() throw();

            Library &lib;
            Lua::VM &lvm;

            template <typename EXPR> inline
            LuaEquilibria & operator<<(const EXPR &expr)
            {
                load(expr,lib,lvm);
                return *this;
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(LuaEquilibria);

        };

    }

}

#endif
