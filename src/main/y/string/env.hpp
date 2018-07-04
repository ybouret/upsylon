
//! \file env.hpp

#ifndef Y_ENV_INCLUDED
#define Y_ENV_INCLUDED 1

#include "y/string.hpp"

namespace upsylon {
    
    struct environment
    {

        static bool get( string &value, const string &name );
        static void set( const string &name, const string &value);
        static void get( void (*proc)( const string &name, const string &value, void *), void *args );

#if 0
        //! check and parse value if it exists
        /**
         implemented for bool, int
         \return true is env 'name' exists
         */
        template <typename T>
        static bool check( T &value, const string &name);
#endif
    };


}

#endif
