//! \file
#ifndef Y_MATH_SIGNAL_SPLINE_INCLUDED
#define Y_MATH_SIGNAL_SPLINE_INCLUDED 1

#include "y/sequence/vector.hpp"

namespace upsylon
{
    namespace math
    {

        //! spline operations
        struct spline
        {

            //! compute array of second derivatives assuming ordered X
            template <typename T> static inline
            void compute(array<T>       &y2,
                         const array<T> &x,
                         const array<T> &y,
                         const bool lower_natural,
                         const T    lower_slope,
                         const bool upper_natural,
                         const T    upper_slope)
            {
                static const T half  = T(0.5);
                static const T three = T(3);
                static const T one   = T(1);
                static const T two   = T(2);
                static const T six   = T(6);

                assert(x.size()>1);
                assert(x.size()==y.size());
                assert(x.size()==y2.size());
                const size_t   n=x.size();

                //______________________________________________________________
                //
                // prepare boundary conditions
                //______________________________________________________________
                const size_t     nm1 = n-1;
                vector<T>        u(nm1);

                if(lower_natural)
                {
                    y2[1]=u[1]=0;
                }
                else
                {
                    y2[1] = -half;
                    u[1]=(three/(x[2]-x[1]))*((y[2]-y[1])/(x[2]-x[1])-lower_slope);
                }

                T qn=0,un=0;
                if(!upper_natural)
                {
                    qn = half;
                    un = (three/(x[n]-x[n-1]))*(upper_slope-(y[n]-y[n-1])/(x[n]-x[n-1]));
                }

                //______________________________________________________________
                //
                //  loop forward
                //______________________________________________________________
                for(size_t i=2,im=1,ip=3;i<n;++i,++im,++ip)
                {
                    const T sig=(x[i]-x[im])/(x[ip]-x[im]);
                    const T p=sig*y2[im]+two;
                    y2[i]=(sig-one)/p;
                    const T tmp = (y[ip]-y[i])/(x[ip]-x[i]) - (y[i]-y[im])/(x[i]-x[im]);
                    u[i]=(six*tmp/(x[ip]-x[im])-sig*u[im])/p;
                }

                //______________________________________________________________
                //
                // loop backwards
                //______________________________________________________________
                y2[n]=(un-qn*u[nm1])/(qn*y2[nm1]+one);
                for(size_t k=nm1,kp=n;k>0;--k,--kp)
                {
                    y2[k]=y2[k]*y2[kp]+u[k];
                }
            }


            //! evaluate spline 
            template <typename T> static inline
            T eval(const T x, const array<T> &X, const array<T> &Y, const array<T> &Y2)
            {
                static const T six   = T(6);

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
                            const T h  = x1-x0;
                            const T a=(x1-x)/h;
                            const T b=(x-x0)/h;
                            return a*y0+b*y1+((a*a*a-a)*Y2[jlo]+(b*b*b-b)*Y2[jup])*(h*h)/six;
                        }
                    }
                }
            }

        };

    }

}

#endif

