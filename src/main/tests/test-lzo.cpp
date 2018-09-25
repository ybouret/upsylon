#include "y/codec/lzo.hpp"
#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ios/osstream.hpp"
#include "y/sequence/vector.hpp"
#include "y/hashing/sha1.hpp"

using namespace upsylon;


Y_UTEST(lzo)
{
    hashing::sha1 H;
    miniLZO & LZO = miniLZO::instance();
    std::cerr << "LZO.VersionString = " << LZO.VersionString() << std::endl;
    std::cerr << "LZO.VersionDate   = " << LZO.VersionDate()   << std::endl;

    if(argc>1&&0==strcmp("run",argv[1]))
    {


        vector<char> content(1024*1024,as_capacity);
        {
            ios::icstream fp( ios::cstdin );
            char C=0;
            while(fp.query(C))
            {
                content << C;
            }
        }
        const uint64_t org = H.key<uint64_t>(content);
        std::cerr << "org=" << org << std::endl;
        {
            ios::ocstream fp("minilzo.bin");
            LZO.Compress(fp,content);
        }
        content.release();

        {
            ios::icstream fp("minilzo.bin");
            const string loaded = LZO.Decompress(fp);
            std::cerr << "#loaded=" << loaded.size() << std::endl;
            const uint64_t lzh = H.key<uint64_t>(loaded);
            std::cerr << "lzh=" << lzh << std::endl;
            Y_CHECK(lzh==org);
        }

    }



}
Y_UTEST_DONE()

