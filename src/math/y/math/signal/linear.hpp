#ifndef Y_MATH_SIGNAL_LINEAR_INCLUDED
#define Y_MATH_SIGNAL_LINEAR_INCLUDED 1

#include "y/sequence/array.hpp"

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
        };

    }
}

#endif

