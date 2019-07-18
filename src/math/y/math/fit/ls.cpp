#include "y/math/fit/ls.hpp"

namespace upsylon
{
    namespace math
    {
        namespace Fit
        {

            LeastSquares_:: ~LeastSquares_() throw()
            {
            }

            LeastSquares_:: LeastSquares_() throw()
            {
            }

            const char * LeastSquares_:: converged_text(const bool flag) throw()
            {
                return (flag? "converged" : "not converged");
            }
        }

    }

}

