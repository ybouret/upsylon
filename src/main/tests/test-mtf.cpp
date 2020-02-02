
#include "y/codec/mtf.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(mtf)
{
    information::mtf_encoder enc;
    information::mtf_decoder dec;

    for(size_t i=100000;i>0;--i)
    {
        const uint8_t x = alea.full<uint8_t>();
        const uint8_t y = enc.fetch(x);
        const uint8_t z = dec.fetch(y);
        Y_ASSERT(x==z);
    }

}
Y_UTEST_DONE()

