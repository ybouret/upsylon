
#include "y/mkl/kernel/lambda.hpp"
#include "y/mkl/types.hpp"

namespace upsylon
{
    namespace mkl
    {

        namespace kernel
        {
            template <> const int lambdas<float>::pmin = -int(numeric<float>::dig)-1;
            template <> const int lambdas<float>::pmax =  int(numeric<float>::max_10_exp);

            template <> const int lambdas<double>::pmin = -int(numeric<double>::dig)-1;
            template <> const int lambdas<double>::pmax =  int(numeric<double>::max_10_exp);

            template <> int lambdas<float>::clamped(const int p) throw()
            {
                return clamp<int>(pmin,p,pmax);
            }

            template <> int lambdas<double>::clamped(const int p) throw()
            {
                return clamp<int>(pmin,p,pmax);
            }

        }

    }

}

