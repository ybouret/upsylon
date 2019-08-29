#include "y/ios/ovstream.hpp"
#include "y/ios/osstream.hpp"

#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(mstreams)
{
    ios::ovstream           vfp;
    ios::embedded_osstream  sfp;

    for(size_t i=5+alea.leq(20);i>0;--i)
    {
        const uint8_t u = alea.full<uint8_t>();
        vfp.write(u);
        sfp.write(u);
    }

    vfp.flush();
    sfp.flush();
    
}
Y_UTEST_DONE()

