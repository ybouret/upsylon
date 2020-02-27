#include "y/information/modulation/mtf.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;
using namespace Information;

Y_UTEST(mtf)
{
    Y_UTEST_SIZEOF(MoveToFrontEncoder);
    Y_UTEST_SIZEOF(MoveToFrontDecoder);
    Y_UTEST_SIZEOF(MoveToFront::node_t);

    MoveToFrontEncoder enc;
    MoveToFrontDecoder dec;

    for(size_t i=100000;i>0;--i)
    {
        const uint8_t x = alea.full<uint8_t>();
        const uint8_t y = enc.fetch(x);
        const uint8_t z = dec.fetch(y);
        Y_ASSERT(x==z);
    }

}
Y_UTEST_DONE()

