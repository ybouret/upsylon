#include "y/ios/bin2dat.hpp"
#include "y/utest/run.hpp"

#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;

Y_UTEST(bin2dat)
{
    if(argc>1&& 0==strcmp("run",argv[1]))
    {
        size_t w = 16;
        ios::icstream inp( ios::cstdin  );
        ios::ocstream out( ios::cstdout );
        char C=0;
        ios::bin2dat  b2d(w);
        while(inp.query(C))
        {
            b2d.write(out,C,false);
        }
        b2d.write(out,0,true);
    }
}
Y_UTEST_DONE()


