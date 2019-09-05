//! \file
#ifndef Y_INTS_DISPLAY_INCLUDED
#define Y_INTS_DISPLAY_INCLUDED 1

#include "y/type/ints.hpp"
#include <iosfwd>

namespace upsylon
{
    

    struct display_int
    {
        //! display [un]singed integer
        static std::ostream & any(std::ostream &os,
                                  const void  *addr,
                                  const size_t size,
                                  const bool   sgnd);

        static  std::ostream & sep(std::ostream &os,
                                   const char   *separator );

        //! wrapper to display any int
        template <typename T> static inline
        std::ostream & to(std::ostream &os, const T x)
        {
            return any(os, &x, sizeof(T), is_signed<T>::value );
        }

        //! wrapper to display some arrays
        template <typename T> static inline
        std::ostream & to(std::ostream &os,
                          const T      *first,
                          const size_t  count,
                          const char   *separator=0)
        {
            assert( !(0==first&&count>0) );
            if(count)
            {
                to(os,first[0]);
                for(size_t i=1;i<count;++i)
                {
                    sep(os,separator);
                    to(os,first[i]);
                }
            }
            return os;
        }

    };




    
}


#endif

