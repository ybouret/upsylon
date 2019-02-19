//! \file
#ifndef Y_MATH_FCN_DERIVATIVE_INCLUDED
#define Y_MATH_FCN_DERIVATIVE_INCLUDED 1

#include "y/math/round.hpp"
#include "y/container/matrix.hpp"
#include "y/exceptions.hpp"
#include <cerrno>

namespace upsylon
{

    namespace math
    {
        //! derivative computation
        template <typename T,const size_t NTAB=16>
        class derivative
        {
        public:

            //! constructor
            inline explicit derivative() : a(NTAB,NTAB) {}

            //! destructor
            inline virtual ~derivative() throw() {}

            //! regularize step and check underflow
            static inline T regularize( const T x, const T h )
            {
                assert(h>=0);
                volatile T temp = x+h;
                const    T step = temp-x;
                if( step<=0 ) throw libc::exception(EDOM,"derivative underflow");
                return step;
            }

            //! step scaling w.r.t characteristic scale of 1
            static inline T unit_step_size() throw()
            {
                static const T value = log_round_floor(__pow(numeric<T>::epsilon,T(1.0)/T(3.0)));
                return value;
            }

            //! estimate derivative by Ridder's method
            /**
             if x_c is the characteristic scaling of f variations, then
             h = x_c * unit_step_size() is a good choice
             */
            template <typename FUNC>
            inline T diff( FUNC &f, const T x, const T h, T &err )
            {
                static const T CON = T(1.2);
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
                        const T errt=max_of<T>(fabs_of(a[j][i]-a[jm][i]),fabs_of(a[j][i]-a[jm][im]));
                        if(!(errt>=0))
                        {
                            throw libc::exception(EDOM, "invalid derivative @%.15g", double(x));
                        }
                        if( ini || (errt<=err) )
                        {
                            err = errt;
                            ans = a[j][i];
                            ini = false;
                        }
                    }
                    assert(!ini);
                    assert(err>=0);
                    if(fabs_of(a[i][i]-a[im][im]) >= (err+err) ) break; // higher order error increases
                }
                return ans;
            }

            //! best effort first order derivative
            template <typename FUNC>
            inline T diff( FUNC &f, const T x, T h )
            {
                static const T max_ftol = log_round_ceil( sqrt_of( numeric<T>::epsilon ) );
                static const T CON = T(1.2);
                // initialize
                T err  = 0;
                T dFdx = diff(f,x,h,err);
                // try to decrease h
                while(err>max_ftol*fabs_of(dFdx) )
                {
                    T       new_err  = 0;
                    const T new_dFdx = diff(f,x,h/=CON,new_err);
                    if(new_err>=err)
                    {
                        break; // not better
                    }
                    err  = new_err;
                    dFdx = new_dFdx;
                }
                return dFdx;
            }

            
            //! local computation of second order derivative
            template <typename FUNC>
            inline T diff2( const T f0, FUNC &f, const T x, const T h, T &err )
            {
                static const T CON = T(1.2);
                static const T CON2 = CON*CON;
                assert(h>=0);
                T    hh  = regularize(x,h);
                T    ww  = hh*hh;
                T    ans = a[1][1] = ( (f(x+hh)-f0)+(f(x-hh)-f0))/ww;
                bool ini = true;
                err      = -1;
                for(size_t i=2,im=1;i<=NTAB;++i,++im)
                {
                    hh      = regularize(x,hh/CON);
                    ww      = hh*hh;
                    a[1][i] = ( (f(x+hh)-f0)+(f(x-hh)-f0))/ww;
                    T fac   = CON2;
                    for(size_t j=2,jm=1;j<=i;++j,++jm)
                    {
                        a[j][i]=(a[jm][i]*fac-a[jm][im])/(fac-T(1));
                        fac=CON2*fac;
                        const T errt=max_of<T>(fabs_of(a[j][i]-a[jm][i]),fabs_of(a[j][i]-a[jm][im]));
                        if( ini || (errt<=err) )
                        {
                            err = errt;
                            ans = a[j][i];
                            ini = false;
                        }
                    }
                    assert(!ini);
                    assert(err>=0);
                    if(fabs_of(a[i][i]-a[im][im]) >= (err+err) ) break; // higher order error increases
                }
                return ans;
            }

            //! best effort second order derivative
            template <typename FUNC>
            inline T diff2( FUNC &f, const T x, T h )
            {
                static const T max_ftol = log_round_ceil( sqrt_of( numeric<T>::epsilon ) );
                static const T CON(1.2);
                // initialize
                const T f0     = f(x);
                T       err    = 0;
                T       d2Fdx2 = diff2(f0,f,x,h,err);
                // try to decrease h
                while(err>max_ftol*fabs_of(d2Fdx2) )
                {
                    T       new_err  = 0;
                    const T new_d2Fdx2 = diff2(f0,f,x,h/=CON,new_err);
                    if(new_err>=err)
                    {
                        break; // not better
                    }
                    err  = new_err;
                    d2Fdx2 = new_d2Fdx2;
                }
                return d2Fdx2;
            }

        private:
            matrix<T> a;
            Y_DISABLE_COPY_AND_ASSIGN(derivative);
        };
    }

}

#endif
