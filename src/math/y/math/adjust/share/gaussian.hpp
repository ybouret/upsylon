
//! \file

#ifndef Y_MATH_ADJUST_GAUSS_INCLUDED
#define Y_MATH_ADJUST_GAUSS_INCLUDED 1

#include "y/math/adjust/share/common.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {

    namespace math {


        namespace Adjust {
            
            template <typename T>
            class Gauss : public Common
            {
            public:
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
                        { const string sig = prefix + "aig" + suffix; ids.push_back(sig); }
                    }

                }

                inline virtual ~Gauss() throw() {}


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Gauss);
            };


        }

    }
}

#endif
