//! \file
#ifndef Y_MATH_ODE_RK4_INCLUDED
#define Y_MATH_ODE_RK4_INCLUDED 1

#include "y/math/ode/types.hpp"
#include "y/math/kernel/tao.hpp"
#include "y/core/loop.hpp"

namespace upsylon
{
    namespace math
    {
        namespace ODE
        {
            //! RK4 integrator
            template <typename T>
            class RK4 : public Field<T>::Arrays
            {
            public:
                typedef typename Field<T>::Array    Array;    //!< Array for variables
                typedef typename Field<T>::Equation Equation; //!< Equation
                typedef typename Field<T>::Callback Callback; //!< normalizing callback

                //! constructor
                inline RK4() :
                Field<T>::Arrays(5),
                Yt( this->next() ),
                k1( this->next() ),
                k2( this->next() ),
                k3( this->next() ),
                k4( this->next() )
                {
                }

                //! destructor
                inline ~RK4() throw()
                {
                }

                //! integrate from X0 to X1
                inline void operator()( Equation &F, Array &Y, const T X0, const T X1, Callback *cb=0 )
                {
                    const T h    = X1-X0;
                    const T half = T(0.5)*h;
                    const T scal = h/T(6);
                    const T Xmid = X0+half;

                    //__________________________________________________________
                    //
                    // k1 = F(X0,Y)
                    //__________________________________________________________
                    F(k1,X0,Y);

                    //__________________________________________________________
                    //
                    // k2 = F(X0+h/2,Y+h/2*k1)
                    //__________________________________________________________
                    tao::setprobe(Yt, Y, half, k1);
                    if(cb) (*cb)(Yt,Xmid);
                    F(k2,Xmid,Yt);

                    //__________________________________________________________
                    //
                    // k3 = F(X0+h/2,Y+h/2*k2)
                    //__________________________________________________________
                    tao::setprobe(Yt,Y,half,k2);
                    if(cb) (*cb)(Yt,Xmid);
                    F(k3,Xmid,Yt);

                    //__________________________________________________________
                    //
                    // k4 = F(X0+h=X1,Y+h*k3)
                    //__________________________________________________________
                    tao::setprobe(Yt, Y, h, k3);
                    if(cb) (*cb)(Yt,X1);
                    F(k4,X1,Yt);

                    //__________________________________________________________
                    //
                    // Y += h/6*(k1+2*k2+2*k3+k4)
                    //__________________________________________________________
                    
                    //! compute the estimate
#define YMK_RK4_UPDATE(I) const T _k2 = k2[I]; const T _k3 = k3[I]; Y[I] += scal * (k1[I]+(_k2+_k2)+(_k3+_k3)+k4[I])
                    Y_LOOP_FUNC_(Yt.size(), YMK_RK4_UPDATE, 1);
//#undef  YMK_RK4_UPDATE
                }

            private:
                Array &Yt;
                Array &k1;
                Array &k2;
                Array &k3;
                Array &k4;


                Y_DISABLE_COPY_AND_ASSIGN(RK4);

            public:
                //Array &YY; //!< may be used by user
            };

        }
    }
}

#endif
