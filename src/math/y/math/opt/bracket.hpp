#ifndef Y_OPT_BRACKET_INCLUDED
#define Y_OPT_BRACKET_INCLUDED 1

#include "y/math/triplet.hpp"
#include "y/math/types.hpp"
#include <iostream>

namespace upsylon
{
    namespace math
    {
        struct bracket
        {
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
                    if( __fabs(delta) <= 0 )
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
                    x.shift(x.b + GOLD * (x.b - x.a));
                    f.shift(F(x.c));
                    assert(f.b<=f.a);
                    assert(x.is_ordered());
                }

            }
        };
    }
}

#endif

