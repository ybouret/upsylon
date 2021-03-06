//! \file
#ifndef Y_MKL_OPT_BRACKET_INCLUDED
#define Y_MKL_OPT_BRACKET_INCLUDED 1

#include "y/mkl/triplet.hpp"
#include "y/mkl/types.hpp"
#include "y/type/cswap.hpp"
#include <iostream>

namespace upsylon {

    namespace mkl {
        
        //! bracketing method
        struct bracket
        {
            //! check minimum bracketing triplet
            template <typename T> static inline
            bool is_valid( triplet<T> &f ) throw()
            {
                return (f.b<=f.a) && (f.b<=f.c);
            }


            //! starting from x.a,x.b and f.a,f.b, search a bracketing triplet
            template <typename T,typename FUNC> static inline
            void expand( FUNC &F, triplet<T> &x, triplet<T> &f)
            {
                static const T GOLD = numeric<T>::gold;
                static const T GLIM = T(10.0);
                static const T TINY = T(1e-20);

                //--------------------------------------------------------------
                // assume f.a and f.b are computed
                //--------------------------------------------------------------
                if (f.b > f.a)
                {
                    cswap(x.a,x.b);
                    cswap(f.a,f.b);
                }
                assert(f.b<=f.a);

                //--------------------------------------------------------------
                // now, we go downhill: initialize the third point
                //--------------------------------------------------------------
                x.c = x.b + GOLD * (x.b - x.a);
                f.c = F(x.c);

                while( f.b > f.c )
                {
                    assert(f.b<=f.a);
                    assert(f.c<f.b);
                    assert(x.is_ordered());

                    const T delta = x.c - x.a;
                    if( fabs_of(delta) <= 0 )
                    {
                        f.a = f.b = f.c;
                        x.a = x.b = x.c;
                        return;
                    }

                    //----------------------------------------------------------
                    // compute geometrical factors
                    //----------------------------------------------------------
                    const T alpha = (x.b - x.a) / delta; assert(alpha>=0);
                    const T beta  = (x.c - x.b) / delta; assert(beta>=0);
                    const T A     =  f.a - f.b;          assert(A>=0);
                    const T B     =  f.b - f.c;          assert(B>0);

                    //----------------------------------------------------------
                    // compute curvature times determinant>0
                    //----------------------------------------------------------
                    const T q     = beta * A - alpha * B;

                    if( q > 0 )
                    {

                        //------------------------------------------------------
                        // compute the probe and its limit
                        //------------------------------------------------------
                        const T p    = alpha*alpha*B + beta*beta*A; assert(p>=0);
                        const T lam  = p / ( 2 * max_of(q,TINY) );  assert(lam>=0);
                        const T u    = x.b + lam * delta;
                        const T ulim = x.b + GLIM * (x.c - x.b);


                        if( (u-x.b)*(x.c-u) >= 0 )
                        {
                            //--------------------------------------------------
                            // parabolic fit between between b and c
                            //--------------------------------------------------
                            const T fu = F(u);
                            if( fu <= f.c )
                            {
                                x.a = x.b; x.b = u;
                                f.a = f.b; f.b = fu;
                                assert(f.b<=f.a);
                                assert(x.is_ordered());
                                continue; // winner
                            }

                            if( fu >= f.b )
                            {
                                x.c = u;
                                f.c = u;
                                assert(f.b<=f.a);
                                assert(x.is_ordered());
                                continue; // winner
                            }

                            goto PROBE; // no interest
                        }

                        if( (u-x.c) * (ulim -u ) >= 0 )
                        {
                            //--------------------------------------------------
                            // between c and ulim
                            //--------------------------------------------------
                            const T fu   = F(u);
                            x.shift(u);
                            f.shift(fu);
                            assert(f.b<=f.a);
                            assert(x.is_ordered());
                            continue;
                        }

                        //------------------------------------------------------
                        // beyond ulim
                        //------------------------------------------------------
                        x.shift(ulim);
                        f.shift(F(ulim));
                        assert(f.b<=f.a);
                        assert(x.is_ordered());
                        continue;
                    }

                    //----------------------------------------------------------
                    // default magnification step
                    //----------------------------------------------------------
                PROBE:
                    //std::cerr << "x0=" << x << std::endl;
                    x.shift(x.c + GOLD * (x.c - x.b));
                    //std::cerr << "x1=" << x << std::endl;

                    f.shift(F(x.c));
                    assert(f.b<=f.a);
                    assert(x.is_ordered());
                }

            }

            //! bracket minium inside x.a,x.c with f.a, f.c computed
            template <typename T, typename FUNC> static inline
            void inside(FUNC       & func,
                        triplet<T> & x,
                        triplet<T> & f )
            {

                static const T        SHRINK = T(0.3819660112501051);
                static const unsigned GTA    = 0x01;
                static const unsigned GTC    = 0x02;
                //______________________________________________________________
                //
                // ordering..
                //______________________________________________________________
                if(x.a>x.c)
                {
                    cswap(x.a, x.c);
                    cswap(f.a, f.c);
                }
                assert(x.a<=x.c);

                //______________________________________________________________
                //
                // try to find x.b so that f.b <= f.a and f.b <= f.c
                // and at least one turn!
                //______________________________________________________________
                T width = x.c-x.a; assert(width>=0);
                for(;;)
                {
                    //__________________________________________________________
                    //
                    // choose x.b
                    //__________________________________________________________

                    if(f.a < f.c )
                    {
                        x.b = clamp<T>(x.a,x.a + SHRINK*(x.c-x.a),x.c);
                        assert(x.is_ordered());
                    }
                    else
                    {
                        if(f.c<f.a)
                        {
                            x.b = clamp<T>(x.a,x.c - SHRINK*(x.c-x.a),x.c);
                            assert(x.is_ordered());
                        }
                        else
                        {
                            // f.c==f.a numerically
                            x.b = clamp<T>(x.a,x.a + T(0.5) * (x.c-x.a),x.c);
                            assert(x.is_ordered());
                        }
                    }
                    assert(x.is_ordered());
                    f.b = func(x.b);

                    unsigned    flag  = 0x00;
                    if(f.b>f.c) flag |= GTC; else { assert(f.b<=f.c); }
                    if(f.b>f.a) flag |= GTA; else { assert(f.b<=f.a); }

                    switch(flag)
                    {

                        case GTA:
                            assert(f.b<=f.c);
                            assert(f.b>f.a);
                            // move c to b
                            x.c = x.b;
                            f.c = f.b;
                            break;

                        case GTC:
                            assert(f.b<=f.a);
                            assert(f.b>f.c);
                            // move a to b
                            x.a = x.b;
                            f.a = f.b;
                            break;

                        case GTA|GTC:
                            assert(f.b>f.c);
                            assert(f.b>f.a);
                            // got to make a choice..
                            if(f.a<=f.c)
                            {
                                x.c = x.b;
                                f.c = f.b;
                            }
                            else
                            {
                                x.a=x.b;
                                f.a=f.b;
                            }
                            break;


                        default:
                            assert(f.b<=f.a);
                            assert(f.b<=f.c);
                            // early return
                            return ;
                    }

                    const T new_width = x.c-x.a;
                    if(new_width>=width)
                    {
                        // one min is numerically on the side
                        break;
                    }

                    width = new_width;
                }

                // prepare the triplet so that minimize shall work...
                if( f.a <= f.c )
                {
                    x.b = x.c = x.a;
                    f.b = f.c = f.a;
                }
                else
                {
                    x.b = x.a = x.c;
                    f.b = f.a = f.c;
                }
            }


        };
    }
}

#endif

