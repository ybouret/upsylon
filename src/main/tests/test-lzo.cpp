#include "y/codec/lzo.hpp"
#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/osstream.hpp"

using namespace upsylon;


Y_UTEST(lzo)
{
    miniLZO & LZO = miniLZO::instance();
    std::cerr << "LZO.VersionString = " << LZO.VersionString() << std::endl;
    std::cerr << "LZO.VersionDate   = " << LZO.VersionDate()   << std::endl;

    if(argc>1&&0==strcmp("run",argv[1]))
    {
        string source;
        {
            ios::icstream fp( ios::cstdin );
            ios::osstream op( source );
            char C=0;
            while(fp.query(C))
            {
                op.write(C);
            }
            op.flush();
        }
        string target = LZO.Compress(source);
        string second = LZO.Decompress(source.size(),target);

    }



}
Y_UTEST_DONE()

