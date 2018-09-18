//! \file
#ifndef Y_LUA_VM_INCLUDED
#define Y_LUA_VM_INCLUDED 1

#include "y/lua/lua.hpp"
#include "y/ptr/arc.hpp"
#include "y/ios/istream.hpp"
#include "y/exception.hpp"

namespace upsylon
{

    namespace Lua
    {

#define Y_LUA_CHECK(CODE) do { if( CODE ) throw exception("@%s: %s", #CODE, lua_tostring(L,-1) ); } while(false)

        class State : public counted_object
        {
        public:
            typedef arc_ptr<State> Pointer;

            explicit State();
            virtual ~State() throw();

            void doFile(   const string &filename );
            void doString( const string &code     );

        private:
            lua_State *L;
            Y_DISABLE_COPY_AND_ASSIGN(State);
        };

    }

}

#endif

