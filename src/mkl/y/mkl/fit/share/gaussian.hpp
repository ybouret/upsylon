
//! \file

#ifndef Y_MATH_ADJUST_GAUSS_INCLUDED
#define Y_MATH_ADJUST_GAUSS_INCLUDED 1

#include "y/math/fit/share/common.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {

    namespace math {

        namespace Fit {

            //! set of Gaussian function
            template <typename T>
            class Gauss : public Common
            {
            public:
                //! setup 'num' gaussians
                inline explicit Gauss( const size_t num, const char *pfx ) :
                Common( 3*num )
                {
                    sequence<string> &ids    = aliasing::_(names);
                    const string      prefix = pfx;

                    for(size_t i=1;i<=num;++i)
                    {
                        const string suffix = Suffix(i);
                        { const string A   = prefix + "A"   + suffix; ids.push_back(A);   }
                        { const string mu  = prefix + "mu"  + suffix; ids.push_back(mu);  }
                        { const string sig = prefix + "sig" + suffix; ids.push_back(sig); }
                    }

                }

                //! cleanup
                inline virtual ~Gauss() throw() {}


                //! compute the sum of gaussians
                inline T compute(const T              x,
                                 const accessible<T> &aorg,
                                 const Variables     &vars) const
                {
                    assert( names.size() > 0 );
                    assert( 0 == (names.size()%3) );

                    T ans = 0;
                    for(size_t g=names.size()/3,i=0;g>0;--g)
                    {
                        const T A   = vars( aorg, names[++i] );
                        const T mu  = vars( aorg, names[++i] );
                        const T sig = vars( aorg, names[++i] );
                        const T dx  = mu-x;
                        const T s2  = sig*sig;
                        ans += A * exp_of( - (dx*dx)/(s2+s2) );
                    }
                    return ans;
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Gauss);
            };


        }

    }
}

#endif
