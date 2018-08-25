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
                //______________________________________________________________
                //
                // initialize direction
                //______________________________________________________________
                if(f.a<f.b)
                {
                    cswap(f.a,f.b);
                    cswap(x.a,x.b);
                }
                assert(f.b<=f.a);
                //______________________________________________________________
                //
                // and first look up
                //______________________________________________________________
                f.c = F( x.c = x.b + numeric<T>::gold * (x.b-x.a) );
                std::cerr << "-- x=" << x << ", f=" << f << std::endl;
                while( f.b>f.c )
                {
                    assert(f.b<=f.a); assert(f.c<f.b); assert(f.c<f.a);
                    assert(x.is_ordered());
                    if(f.b<(f.a+f.c)/2)
                    {
                        std::cerr << "should try parabolic" << std::endl;
                        const T W = x.c-x.a;
                        const T H = f.a-f.c; assert(H>0);

                        exit(1);
                    }
                    else
                    {
                        // forward
                        x.a = x.b; x.b=x.c;
                        f.a = f.b; f.b=f.c;
                        f.c = F( x.c = x.b + numeric<T>::gold * (x.b-x.a) );
                    }


                    std::cerr << "-- x=" << x << ", f=" << f << std::endl;
                }

                assert(is_valid(f));
            }

        };
    }
}

#endif

