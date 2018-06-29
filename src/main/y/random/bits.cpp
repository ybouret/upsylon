#include "y/random/bits.hpp"

namespace upsylon
{

    namespace random
    {
        bits:: ~bits() throw()
        {
        }

        bits:: bits(const uint32_t maxValue) throw() :
        span(maxValue),
        nbit(bits_for(span)),
        half(span>>1),
        denD(1.0+double(span)),
        denF(1.0f+float(span))
        {
        }

        template <>
        float bits:: to<float>() throw()
        {
            return (0.5f + float( next32() ))/denF;
        }

        template <>
        double bits:: to<double>() throw()
        {
            return (0.5 + double( next32() ))/denD;
        }
    }

}

