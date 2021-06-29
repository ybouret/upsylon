
//! \file

#ifndef Y_HUNT_INCLUDED
#define Y_HUNT_INCLUDED 1

#include "y/sequence/accessible.hpp"
#include "y/type/utils.hpp"

namespace upsylon
{

    //__________________________________________________________________________
    //
    //
    //! hunting values withing increasing ranges
    //
    //__________________________________________________________________________
    struct hunt
    {
        //! status of the last search
        enum status
        {
            found_below = -1, //!< below smallest value
            found_inner =  0, //!< inner bracket
            found_above =  1, //!< above greates valye
        };


        //______________________________________________________________________
        //
        //
        //! search with hysteresis for x[j] <= xx <= x[j+1], x[1..n]
        /**
         \param xx the value to locate
         \param x  array x[1..n], in INCREASING order
         \param n  objects to match
         \param j  result of a previous search
         */
        //
        //______________________________________________________________________
        template <typename T, typename SOURCE> static inline
        status search(const T xx, SOURCE &x, const size_t n, size_t &j) throw()
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
                        j = track<T,SOURCE>(xx,x,1,jlo);
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
                            j = track<T,SOURCE>(xx,x,jup,n);
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

        //______________________________________________________________________
        //
        //! find for x[1..x.size()]
        //______________________________________________________________________
        template <typename T, typename SOURCE> static inline
        status search(const T xx, SOURCE &x, size_t &j) throw()
        {
            return search<T,SOURCE>(xx,x,x.size(),j);
        }


        //______________________________________________________________________
        //
        //
        //! search WITHOUT hysteresis for x[j] <= xx <= x[j+1], x[1..n]
        /**
         \param xx the value to locate
         \param x  array x[1..n], in INCREASING order
         \param n  objects to match
         \param j  result of a previous search
         */
        //
        //______________________________________________________________________
        template <typename T, typename SOURCE> static inline
        status lookup(const T xx, SOURCE &x, const size_t n, size_t &j) throw()
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

                    j = track(xx,x,1,n);
                    return found_inner;
                }

            }
        }


        //______________________________________________________________________
        //
        //! lookup in accessible type
        //______________________________________________________________________
        template <typename T, typename SOURCE> static inline
        status lookup(const T xx, SOURCE &x,size_t &j) throw()
        {
            return lookup<T,SOURCE>(xx,x,x.size(),j);
        }
        
        //______________________________________________________________________
        //
        //
        //! x[jlo]<=x<=x[jhi] => x[jlo]<=xx<=x[jlo+1]
        //
        //______________________________________________________________________
        template <typename T, typename SOURCE> static inline
        size_t track(const T xx, SOURCE &x, size_t jlo, size_t jhi) throw()
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
        
    };
    
    

}

#endif
