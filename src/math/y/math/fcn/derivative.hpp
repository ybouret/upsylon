//! \file
#ifndef Y_MATH_FCN_DERIVATIVE_INCLUDED
#define Y_MATH_FCN_DERIVATIVE_INCLUDED 1

#include "y/math/types.hpp"
#include "y/container/matrix.hpp"
#include "y/exception.hpp"

namespace upsylon
{

    namespace math
    {
        template <typename T,const size_t NTAB=16>
        class derivative
        {
        public:

            inline explicit derivative() : a(NTAB,NTAB) {}
            inline virtual ~derivative() throw() {}

            //! regularize step
            static inline T regularize( const T x, const T h )
            {
                assert(h>=0);
                volatile T temp = x+h;
                const    T step = temp-x;
                if( step<=0 ) throw exception("derivative underflow");
                return step;
            }

            //! choose step scaling w.r.t characteristic scale of 1
            static inline T step_size() throw()
            {
                static const T scaling_factor = __pow(numeric<T>::epsilon,T(1.0)/T(3.0));
                return scaling_factor;
            }

            //! estimate derivative by Ridder's method
            template <typename FUNC>
            inline T compute( FUNC &f, const T x, const T h, T &err )
            {
                static const T CON(1.2);
                static const T CON2 = CON*CON;
                assert(h>=0);
                T    hh  = regularize(x,h);
                T    ww  = hh+hh;
                T    ans = a[1][1] = (f(x+hh)-f(x-hh))/ww;
                bool ini = true;
                err      = -1;
                for(size_t i=2,im=1;i<=NTAB;++i,++im)
                {
                    hh      = regularize(x,hh/CON);
                    ww      = hh+hh;
                    a[1][i] = (f(x+hh)-f(x-hh))/ww;
                    T fac   = CON2;
                    for(size_t j=2,jm=1;j<=i;++j,++jm)
                    {
                        a[j][i]=(a[jm][i]*fac-a[jm][im])/(fac-T(1));
                        fac=CON2*fac;
                        const T errt=max_of<T>(__fabs(a[j][i]-a[jm][i]),__fabs(a[j][i]-a[jm][im]));
                        if( ini || (errt<=err) )
                        {
                            err = errt;
                            ans = a[j][i];
                            ini = false;
                        }
                    }
                    assert(!ini);
                    assert(err>=0);
                    if(__fabs(a[i][i]-a[im][im]) >= (err+err) ) break; // higher order error increases
                }
                return ans;
            }

        private:
            matrix<T> a;
            Y_DISABLE_COPY_AND_ASSIGN(derivative);
        };
    }

}

#endif
