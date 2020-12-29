
#include "y/mkl/fitting/lambda.hpp"
#include "y/mkl/types.hpp"

namespace upsylon
{
    namespace mkl
    {

        namespace fitting
        {
            template <> const int lambdas<float>::pmin = -int(numeric<float>::dig)-1;
            template <> const int lambdas<float>::pmax =  int(numeric<float>::max_10_exp);

            template <> const int lambdas<double>::pmin = -int(numeric<double>::dig)-1;
            template <> const int lambdas<double>::pmax =  int(numeric<double>::max_10_exp);

        }

    }

}

