//! \file

#ifndef Y_MATH_ADJUST_POLY_INCLUDED
#define Y_MATH_ADJUST_POLY_INCLUDED 1

#include "y/math/adjust/share/common.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {

    namespace math {

        namespace Adjust {

            template <typename T>
            class Polynomial : public Common
            {
            public:

                inline explicit Polynomial(const size_t degree, const char *pfx) :
                Common( degree+1  )
                {
                    sequence<string> &ids    = aliasing::_(names);
                    const string      prefix = pfx;
                    for(size_t i=0;i<=degree;++i)
                    {
                        const string id     = prefix + Suffix(i);
                        ids.push_back(id);
                    }
                    assert(1+degree==names.size());
                }

                inline virtual ~Polynomial() throw()
                {
                }

             

                inline void compute(const T              x,
                                    const accessible<T> &aorg,
                                    const Variables     &vars) const
                {
                    assert( names.size() > 0 );
                    size_t n = names.size();
                    T      ans = vars(aorg,names[n--]);
                    while(n>0)
                    {
                        ans *= x;
                        ans += vars(aorg,names[n--]);
                    }

                    return ans;
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Polynomial);
            };


        }

    }
}

#endif
