
#include "y/math/fit/ls-rescale.hpp"

namespace upsylon
{
    namespace math
    {
        namespace Fit
        {

            RescalingInfo:: ~RescalingInfo() throw()
            {
            }

            const char RescalingInfo::_coeff[] = "coeff";
            const char RescalingInfo::_scale[] = "scale";
            const char RescalingInfo::_shift[] = "shift";

            RescalingInfo:: RescalingInfo() :
            vars(NVAR),
            used(3,false)
            {
                Variables &_ = (Variables &)vars;
                _ << _coeff << _scale << _shift;
                assert(NVAR==vars.size());
            }

            
        }
    }
}
