
#include "y/math/fit/ls-rescale.hpp"

namespace upsylon
{
    namespace math
    {
        namespace Fit
        {

            bool & Rescale:: use_coeff() throw() { return vars(used,_coeff); }
            bool & Rescale:: use_scale() throw() { return vars(used,_scale); }
            bool & Rescale:: use_shift() throw() { return vars(used,_shift); }

            
            Rescale:: ~Rescale() throw()
            {
            }

            const char Rescale::_coeff[] = "coeff";
            const char Rescale::_scale[] = "scale";
            const char Rescale::_shift[] = "shift";

            Rescale:: Rescale() :
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
