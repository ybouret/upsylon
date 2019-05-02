
#include "y/math/fit/ls-rescale.hpp"

namespace upsylon
{
    namespace math
    {
        namespace Fit
        {
            RescalerInfo::  RescalerInfo() throw() {}
            RescalerInfo:: ~RescalerInfo() throw() {}

#define Y_LS_RESCALER(FIELD) const char RescalerInfo:: FIELD[] = #FIELD

            Y_LS_RESCALER(FunctionCoeff);
            Y_LS_RESCALER(VariableShift);
            Y_LS_RESCALER(VariableScale);

        }
    }
}
