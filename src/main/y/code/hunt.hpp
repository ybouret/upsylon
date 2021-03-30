
//! \file

#ifndef Y_HUNT_INCLUDED
#define Y_HUNT_INCLUDED 1

#include "y/sequence/accessible.hpp"
#include "y/type/utils.hpp"

namespace upsylon
{

    
    struct hunt
    {
        //______________________________________________________________________
        //
        //
        //! x[jlo]<=x<=x[jhi] => x[jlo]<=xx<=x[jlo+1]
        //
        //______________________________________________________________________
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

        //______________________________________________________________________
        //
        //
        //! look for x[j] <= xx <= x[j+1]
        /**
         \param xx the value to locate
         \param x  array x[1..n], in increasing order
         \param n  objects to match
         \param j  result of a previous search
         */
        //
        //______________________________________________________________________
        template <typename T> static inline
        status find(const T xx, const T x[], const size_t n, size_t &j) throw()
        {
            //__________________________________________________________________
            //
            // routing according to n
            //__________________________________________________________________
            switch(n)
            {
                case  0: j=0; return found_below;                           // bad
                case  1: j=1; return (xx<x[1]) ? found_below : found_above; // bad
                default: assert(n>=2);
                    break;
            }

            if(xx<x[1])
            {
                //______________________________________________________________
                //
                // below smallest value
                //______________________________________________________________
                j=1; return found_below;
            }
            else
            {
                if(xx>x[n])
                {
                    //__________________________________________________________
                    //
                    // above greatest value
                    //__________________________________________________________
                    j=n; return found_above;
                }
                else
                {
                    //__________________________________________________________
                    //
                    // within range
                    //__________________________________________________________
                    const size_t jlo = clamp<size_t>(1,j,n-1);
                    if(xx<x[jlo])
                    {
                        //______________________________________________________
                        //
                        // below current position
                        //______________________________________________________
                        j = track(xx,x,1,jlo);
                        return found_inner;
                    }
                    else
                    {
                        assert(xx>=x[jlo]);
                        const size_t jup = jlo+1;
                        if(xx>x[jup])
                        {
                            //__________________________________________________
                            //
                            // above current position
                            //__________________________________________________
                            j = track(xx,x,jup,n);
                            return found_inner;
                        }
                        else
                        {
                            assert(xx<=x[jup]);
                            //__________________________________________________
                            //
                            // cache!
                            //__________________________________________________
                            j=jlo;
                            return found_inner;
                        }

                    }
                }
            }


        }
        
    };
    
    

}

#endif
