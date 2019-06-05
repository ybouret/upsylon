//! \file
#ifndef Y_MATH_SIGNAL_LINEAR_INCLUDED
#define Y_MATH_SIGNAL_LINEAR_INCLUDED 1

#include "y/sequence/array.hpp"
#include "y/container/sequence.hpp"
#include "y/math/types.hpp"
#include "y/type/cswap.hpp"

namespace upsylon
{
    namespace math
    {

        //! linear operations
        struct linear
        {
            //! linear interpolation, assuming sorted X
            template <typename T> static inline
            T eval( const T x, const array<T> &X, const array<T> &Y ) throw()
            {
                assert(X.size()>0);
                assert(X.size()==Y.size());
                if(x<=X[1])
                    return Y[1];
                else
                {
                    size_t jup = X.size();
                    if(X[jup]<=x)
                    {
                        return Y[jup];
                    }
                    else
                    {
                        assert(x>X[1]);assert(x<X[ X.size() ]);
                        size_t jlo = 1;
                        while(jup-jlo>1)
                        {
                            const size_t jmid = (jlo+jup)>>1;
                            const T      xmid = X[jmid];
                            if(xmid<=x)
                            {
                                jlo = jmid;
                            }
                            else
                            {
                                jup = jmid;
                            }
                        }
                        {
                            assert(X[jlo]<X[jup]);
                            const T x0 = X[jlo];
                            const T x1 = X[jup];
                            const T y0 = Y[jlo];
                            const T y1 = Y[jup];
                            return y0 + ( (x-x0) * (y1-y0) )/(x1-x0);
                        }
                    }
                }
            }

            //! find intercepting values
            template <typename T> static inline
            void zfind( sequence<T> &xx, const T yy, const array<double> &X, const array<double> &Y)
            {
                assert(X.size()==Y.size());
                const size_t n = X.size();
                xx.free();
                for(size_t i=1,ip=2;i<n;++i,++ip)
                {
                    const T y0 = Y[i];
                    const T y1 = Y[ip];
                    T ylo = y0;
                    T yhi = y1;
                    if( yhi<ylo ) cswap(ylo,yhi);
                    if(yy>=ylo&&yy<=yhi)
                    {
                        const T x0 = X[i];
                        const T x1 = X[ip];
                        const T dy = y1-y0;
                        const T x  = ( fabs(dy)>0 ) ? x0 + ((yy-y0)*(x1-x0))/dy : (x0+x1)/2;
                        if( xx.size()<=0 || x>xx.back() )
                        {
                            xx << x;
                        }
                    }
                }
            }

        };

    }
}

#endif

