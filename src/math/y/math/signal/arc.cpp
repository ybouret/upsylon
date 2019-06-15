#include "y/math/signal/arc.hpp"

namespace upsylon
{
    namespace math
    {

        template <>
        void arc::save_real<float>(ios::ostream &fp, const float &x)
        {
            fp("%.15g",x);
        }

        template <>
        void arc::save_real<double>(ios::ostream &fp, const double &x)
        {
            fp("%.15g",x);
        }


    }
}

