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

            template <typename T>
            T get( const string &id );

            template <typename T>
            T to(const int i);


            template <typename T>
            inline T get(const char *id) { const string _(id); return get<T>(_); }

            template <typename T>
            void push( const T & );

            lua_State * operator*() throw() { return L; }

            bool exists( const string &name );

            inline
            bool exists( const char   *name) { const string _(name); return exists(_); }

        private:
            lua_State *L;
            Y_DISABLE_COPY_AND_ASSIGN(State);
        };

        typedef State::Pointer VM;
    }

}

#endif

