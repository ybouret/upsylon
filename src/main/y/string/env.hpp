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

        //! wrapper to get named value
        static bool get( string &value, const char   *name );


        //! set variable 'name' to 'value'
        static void set( const string &name, const string &value);

        //! set variable wrapper
        static void set( const char *name, const string &value);

        //! set variable wrapper
        static void set( const char *name, const char *value);


        //! got over all environment variables
        static void get( void (*proc)( const string &name, const string &value, void *), void *args );

        //! check if name is defined, and convert its value
        /**
         valid for: bool, unit_t, size_t, float, double
         */
        template <typename T>
        static bool check( T &value, const string & name );

        //! wrapper to check<T>
        template <typename T>
        static inline bool check( T &value, const char *name )
        {
            const string _(name); return check<T>(value,_);
        }

        //! true: defined and ok | false: not defined or no
        static bool flag(const string &name);

        //! wrapper
        static bool flag(const char   *name);

    };


}

#endif
