
#include "y/math/fit/fit-conic.hpp"
#include "y/sort/sorted-sum.hpp"
#include "y/math/kernel/lu.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace math
    {
        fit_conic:: ~fit_conic() throw()
        {
        }

        fit_conic:: fit_conic() :
        shape2d(21),
        sums(count),
        C(6,6),
        v(0),
        S(6,6)

        {

            {
                C[1][1] = C[2][2] = C[3][3] = 1; v=1;
            }
        }

        bool fit_conic:: __compute()
        {

            // prepare sums
            {
                shape2d_arrays &self = *this;
                double         *s    = *sums;
                for(size_t i=0;i<count;++i)
                {
                    s[i] = sorted_sum(self[i]);
                }
            }

            //dispatch
            size_t l=0;
            S.ld(0);
            for(size_t i=6;i>0;--i)
            {
                for(size_t j=i;j>0;--j)
                {
                    S[i][j] = S[j][i] = sums[++l];
                }
            }
            std::cerr << "S=" << S << std::endl;
            return false;
        }

    }
}

