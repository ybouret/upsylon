//!\file
#ifndef Y_TYPE_STDS_INCLUDED
#define Y_TYPE_STDS_INCLUDED 1

#include "y/type/ints.hpp"
#include <cstdlib>

namespace upsylon {

    namespace core {
        
        //! calling the right function depending on the type
        template <typename T> struct standard;

        //! for int
        template <> struct standard<int>
        {
            typedef div_t      div_type;           //!< alias
            typedef div_type (*div_proc)(int,int); //!< prototype
            static  const      div_proc div_call;  //!< div
        };
        
        //! for long
        template <> struct standard<long>
        {
            typedef ldiv_t         div_type;             //!< alias
            typedef div_type     (*div_proc)(long,long); //!< prototype
            static  const div_proc div_call;             //!< ldiv
        };
        
        //! for long long
        template <> struct standard<long long>
        {
            typedef lldiv_t        div_type;                       //!< alias
            typedef div_type     (*div_proc)(long long,long long); //!< prototype
            static  const div_proc div_call;                       //!< lldiv
        };
    }

    //! embedding standard APIs
    template <typename T>
    struct standard
    {
        static const size_t                     size = sizeof(T); //!< sizeof(T)
        typedef typename signed_int<size>::type type;             //!< matching signed integer
        typedef core::standard<type>            api;              //!< pointing to API
        typedef typename api::div_type          div_type;         //!< [l[l]]div
    };


}

#endif

