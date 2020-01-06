#include "y/randomized/bits.hpp"
#include "y/type/bzset.hpp"

namespace upsylon
{

    namespace randomized
    {
        bits:: ~bits() throw()
        {
            zforce(span);
            zforce(span);
            zforce(half);
            zforce(denD);
            zforce(denF);
            zforce(symD);
            zforce(symF);
            zforce(midD);
            zforce(midF);

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

        void bits:: fill( void *data, const size_t size ) throw()
        {
            assert(!(NULL==data&&size>0));
            const size_t nw = size / sizeof(uint32_t);
            const size_t nr = size - (nw*sizeof(uint32_t));
            uint8_t     *p  = static_cast<uint8_t *>(data);

            for(size_t i=nw;i>0;--i,p+=sizeof(uint32_t))
            {
                *static_cast<uint32_t *>(static_cast<void *>(p)) = next32();
            }

            {
                const uint32_t w = next32();
                memcpy(p,&w,nr);
            }
        }


    }

}
