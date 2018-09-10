#include "y/codec/base64.hpp"
#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;

Y_UTEST(base64)
{
    if(argc>1&&0==strcmp(argv[1],NULL))
    {
        return 0;
    }

    ios::icstream        fp( ios::cstdin  );
    ios::ocstream        op( ios::cstdout );
    ios::base64::encoder b64;

    char C = 0;
    while( fp.query(C) )
    {
        b64.write(C);
        while(b64.query(C))
        {
            op.write(C);
        }
    }
    b64.flush();
    while(b64.query(C))
    {
        op.write(C);
    }

}
Y_UTEST_DONE()


