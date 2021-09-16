

//! \file

#ifndef Y_CHEMICAL_LUA_EQUILIBRIA_INCLUDED
#define Y_CHEMICAL_LUA_EQUILIBRIA_INCLUDED 1

#include "y/chemical/equilibria.hpp"
#include "y/lua++/state.hpp"

namespace upsylon
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //! Equilibria with link to library and Lua for easy syntax
        //
        //______________________________________________________________________
        class LuaEquilibria : public Equilibria
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            explicit LuaEquilibria(Library &, Lua::VM &); //!< setup
            virtual ~LuaEquilibria() throw();             //!< cleanup


            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! automatic parsing
            template <typename EXPR> inline
            LuaEquilibria & operator<<(const EXPR &expr)
            {
                load(expr,lib,lvm);
                return *this;
            }

            //! load a table of descriptive strings
            void append(const string &tableName);

            //__________________________________________________________________
            //
            // member
            //__________________________________________________________________

            Library &lib; //!< linked library
            Lua::VM &lvm; //!< linked lua_State
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(LuaEquilibria);

        };

    }

}

#endif
