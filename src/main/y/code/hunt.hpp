
//! \file

#ifndef Y_HUNT_INCLUDED
#define Y_HUNT_INCLUDED 1

#include "y/sequence/accessible.hpp"

namespace upsylon
{

    template <typename T>
    size_t hunt(accessible<T> &x, typename type_traits<T>::parameter_type xx)
    {
        const size_t n = x.size();
        switch(n)
        {
            case 0: return 0;
            case 1: return 1;
            default:
                break;
        }

        if(xx<=x[1])
        {
            return 1;
        }
        else
        {
            const size_t m = n-1;
            if(xx>=x[n])
            {
                return m;
            }
            else
            {
                assert(x>=x[1] && xx<=x[n]);
                return 1;
            }
        }

    }

}

#endif
