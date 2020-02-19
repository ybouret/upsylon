#include "y/information/shift/mtf.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;

Y_UTEST(mtf)
{
    Y_UTEST_SIZEOF(information::mtf_encoder);
    Y_UTEST_SIZEOF(information::mtf_decoder);
    Y_UTEST_SIZEOF(information::mtf_modulation::node_t);

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

