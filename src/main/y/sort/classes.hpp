//! \file
#ifndef Y_SORT_CLASSES_INCLUDED
#define Y_SORT_CLASSES_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{
    namespace core
    {

        //! count classes on a SORTED range
        template <typename ITERATOR>
        size_t __classes( ITERATOR curr, const ITERATOR last )
        {
            if( curr != last )
            {
                ITERATOR scan = curr;
                size_t   ncls  = 1;

                while( scan != last )
                {
                    if( *scan != *curr )
                    {
                        ++ncls;
                        curr = scan;
                    }
                    ++scan;
                }
                return ncls;
            }
            else
            {
                return 0;
            }
        }
    }

    //! count classes on a SORTED sequence
    template <typename SEQUENCE>
    size_t classes( const SEQUENCE &seq )
    {
        return core::__classes(seq.begin(),seq.end());
    }

}

#endif

