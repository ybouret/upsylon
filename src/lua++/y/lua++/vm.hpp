//! \file
#ifndef Y_LUA_VM_INCLUDED
#define Y_LUA_VM_INCLUDED 1

#include "y/lua/lua.hpp"
#include "y/ptr/counted.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon
{

    namespace Lua
    {

        class State : public counted_object
        {
        public:
            typedef arc_ptr<State> Pointer;

            explicit State();
            virtual ~State() throw();

        private:
            lua_State *L;
            Y_DISABLE_COPY_AND_ASSIGN(State);
        };

    }

}

#endif

