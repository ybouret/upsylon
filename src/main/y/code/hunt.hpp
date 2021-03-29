
//! \file

#ifndef Y_HUNT_INCLUDED
#define Y_HUNT_INCLUDED 1

#include "y/sequence/accessible.hpp"
#include "y/type/utils.hpp"

namespace upsylon
{


    //! x[j] <= xx <= x[j+1]
    template <typename T>
    void hunt(const T xx, const T x[], const size_t n, size_t &j) throw()
    {
        switch(n)
        {
            case 0: j=0; return; // bad
            case 1: j=1; return; // bad
            default:
                break;
        }
        assert(n>=2);
        const size_t m   = n-1;
        size_t       jlo = clamp<size_t>(1,j,m);
        size_t       jup = jlo+1;

        if(xx<x[jlo])
        {
            if(xx<x[1])
            {
                // extrapolate from x[1]..x[2]
                j=1;
                return;
            }
            else
            {
                jup = jlo;
                jlo = 1;
                goto BISECTION;
            }
        }
        else
        {
            assert(xx>=x[jlo]);
            if(x>x[jup])
            {
                if(x>=x[m])
                {
                    // extrapolate from x[n-1]..x[n]
                    j=m;
                    return;
                }
                else
                {
                    jlo = jup;
                    jup = n;
                    goto BISECTION;
                }

            }
            else
            {
                // xx <= x[jup] => cached
                return;
            }
        }

        BISECTION:
        assert(x[jlo]<=xx); assert(xx<=x[jup]);
        

    }

}

#endif
