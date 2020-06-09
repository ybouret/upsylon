#include "y/math/fit/sample/info.hpp"

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
