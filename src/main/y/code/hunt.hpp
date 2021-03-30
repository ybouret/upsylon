
//! \file

#ifndef Y_HUNT_INCLUDED
#define Y_HUNT_INCLUDED 1

#include "y/sequence/accessible.hpp"
#include "y/type/utils.hpp"

namespace upsylon
{

    
    struct hunt
    {
        //! x[jlo]<=x<=x[jhi] => x[jlo]<=xx<=x[jlo+1]
        template <typename T> static inline
        size_t track(const T xx, const T x[], size_t jlo, size_t jhi) throw()
        {
            assert(xx>=x[jlo]);
            assert(xx<=x[jhi]);
            assert(jlo<jhi);
            while(jhi-jlo>1)
            {
                const size_t jmid = (jlo+jhi)>>1;
                const T      xmid = x[jmid];
                if(xmid<xx)
                {
                    jlo = jmid;
                }
                else
                {
                    if(xx<xmid)
                    {
                        jhi = jmid;
                    }
                    else
                    {
                        return jmid;
                    }
                }
            }
            return jlo;
        }

        enum status
        {
            found_below = -1,
            found_inner =  0,
            found_above =  1,
        };

        //! x[j] <= xx <= x[j+1]
        template <typename T> static inline
        status find(const T xx, const T x[], const size_t n, size_t &j) throw()
        {
            switch(n)
            {
                case  0: j=0; return found_below;                           // bad
                case  1: j=1; return (xx<x[1]) ? found_below : found_above; // bad
                default: assert(n>=2);
                    break;
            }

            if(xx<x[1])
            {
                // below smallest value
                j=1; return found_below;
            }
            else
            {
                if(x>x[n])
                {
                    // above greatest value
                    j=n; return found_above;
                }
                else
                {

                }
            }


#if 0

            const size_t m   = n-1;
            size_t       jlo = clamp<size_t>(1,j,m);
            size_t       jup = jlo+1;
            
            if(xx<x[jlo])
            {
                if(xx<x[1])
                {
                    // extrapolate from x[1]..x[2]
                    j=1;
                    return found_below;
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
                if(xx>x[jup])
                {
                    if(xx>=x[m])
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
            j = track<T>(xx,x,jlo,jup);
#endif
        }
        
    };
    
    

}

#endif
