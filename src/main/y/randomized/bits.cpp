#include "y/randomized/bits.hpp"
#include "y/type/block/zset.hpp"

namespace upsylon
{

    namespace randomized
    {
        bits:: ~bits() throw()
        {
            _bzset(span);
            _bzset(span);
            _bzset(half);
            _bzset(denD);
            _bzset(denF);
            _bzset(symD);
            _bzset(symF);
            _bzset(midD);
            _bzset(midF);

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
            uint8_t *p = static_cast<uint8_t *>(data);
            for(size_t i=0;i<size;++i)
            {
                *(p++) = full<uint8_t>();
            }
        }

        void bits:: fillnz( void *data, const size_t size ) throw()
        {
            assert(!(NULL==data&&size>0));
            uint8_t *p = static_cast<uint8_t *>(data);
            for(size_t i=0;i<size;++i)
            {
                const uint8_t b = full<uint8_t>();
                *(p++) = (b<=0) ? 1 : b;
            }
        }



    }

}
