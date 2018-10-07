#include "y/codec/delta.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(delta)
{
    delta_encoding enc;
    delta_encoding dec;

    for(size_t i=100000;i>0;--i)
    {
        const uint8_t x = alea.full<uint8_t>();
        const uint8_t y = enc.encode(x);
        const uint8_t z = dec.decode(y);
        Y_ASSERT(x==z);
    }
}
Y_UTEST_DONE()
