#include "y/information/modulation/delta.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;
using namespace Information;

Y_UTEST(delta)
{
    Y_UTEST_SIZEOF(DeltaEncoder);
    Y_UTEST_SIZEOF(DeltaEncoder);
    DeltaEncoder enc;
    DeltaDecoder dec;

    for(size_t i=100000;i>0;--i)
    {
        const uint8_t x = alea.full<uint8_t>();
        const uint8_t y = enc.fetch(x);
        const uint8_t z = dec.fetch(y);
        Y_ASSERT(x==z);
    }
}
Y_UTEST_DONE()
