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

        //! checking code wrapper
#define Y_LUA_CHECK(CODE) do { if( CODE ) throw exception("@%s: %s", #CODE, lua_tostring(L,-1) ); } while(false)

        //! lua_State wrapper
        class State : public counted_object
        {
        public:
            typedef arc_ptr<State> Pointer; //!< alias for shared states

            explicit State(); //! setup
            virtual ~State() throw(); //!< destrcutor

            //! load file
            void doFile(   const string &filename );

            //! load code
            inline
            void doFile( const char *filename )
            {
                const string _(filename); doFile(_);
            }

            //! load string
            void doString( const string &code );

            //! load code
            inline
            void doString(const char *code)
            {
                const string _(code); doString(_);
            }

            //! get a variable by its name
            template <typename T>
            T get( const string &id );

            //! helper to convert variable
            template <typename T>
            T to(const int i);

            //! get variable wrapper
            template <typename T>
            inline T get(const char *id) { const string _(id); return get<T>(_); }

            //! push onto stack
            template <typename T>
            void push( const T & );

            //! access to low-level
            lua_State * operator*() throw() { return L; }

            //! check lua variable exists
            bool exists( const string &name );

            //! wrapper to exists
            inline
            bool exists( const char   *name) { const string _(name); return exists(_); }

        private:
            lua_State *L;
            Y_DISABLE_COPY_AND_ASSIGN(State);
        };

        typedef State::Pointer VM; //!< shared lua_State
    }

}

#endif

