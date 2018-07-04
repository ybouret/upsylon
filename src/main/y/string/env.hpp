
//! \file env.hpp

#ifndef Y_ENV_INCLUDED
#define Y_ENV_INCLUDED 1

#include "y/string.hpp"

namespace upsylon {
    
    //! environment access
    struct environment
    {

        //! try to get value of name, true upon success
        static bool get( string &value, const string &name );
        
        //! set variable 'name' to 'value'
        static void set( const string &name, const string &value);
        
        //! got over all environment variables
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
