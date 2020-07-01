//!\file
#ifndef Y_TYPE_DIVS_INCLUDED
#define Y_TYPE_DIVS_INCLUDED 1

#include "y/type/ints.hpp"
#include <cstdlib>

namespace upsylon {

    namespace core {
        
        //! calling the right function depending on the type
        template <typename T> struct standard;

        //! for int
        template <> struct standard<int>
        {
            typedef div_t      div_type; //!< alias
            typedef div_type (*div_proc)(int,int);
            static  const      div_proc div_call;
        };
        
        //! for long
        template <> struct standard<long>
        {
            typedef ldiv_t         div_type; //!< alias
            typedef div_type     (*div_proc)(long,long);
            static  const div_proc div_call;
        };
        
        //! for long lon
        template <> struct standard<long long>
        {
            typedef lldiv_t  div_type; //!< alias
            typedef div_type     (*div_proc)(long,long);
            static  const div_proc div_call;
        };
    }

    template <typename T>
    struct standard
    {
        static const size_t                     size = sizeof(T);
        typedef typename signed_int<size>::type type;
        typedef core::standard<type>            api;
        typedef typename api::div_type          div_type;

    };


}

#endif

