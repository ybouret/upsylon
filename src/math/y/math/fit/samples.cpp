#include "y/math/fit/samples.hpp"

namespace upsylon
{
    namespace math
    {
        namespace Fit
        {
            SampleInfo:: ~SampleInfo() throw() {}

            SampleInfo:: SampleInfo(const size_t nvar_max) :
            variables(nvar_max)
            {
            }
            
        }
    }
}

