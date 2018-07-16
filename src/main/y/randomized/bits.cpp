#include "y/randomized/bits.hpp"

namespace upsylon
{

    namespace randomized
    {
        bits:: ~bits() throw()
        {
        }

        bits:: bits(const uint32_t maxValue) throw() :
        span(maxValue),
        nbit(bits_for(span)),
        half(span>>1),
        denD(1.0+double(span)),
        denF(1.0f+float(span)),
        symD(0.5*denD),
        symF(0.5f*denF),
        midD(0.5*double(span)),
        midF(0.5f*float(span))
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


        template <>
        float bits:: symm<float>() throw()
        {
            return ( float(next32()) - midF ) / symF;
        }

        template <>
        double bits:: symm<double>() throw()
        {
            return ( double(next32()) - midD ) / symD;
        }
    }

}
