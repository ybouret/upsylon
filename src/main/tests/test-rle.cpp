#include "y/information/rle.hpp"
#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"
#include "y/string.hpp"
#include "y/string/convert.hpp"

using namespace upsylon;

Y_UTEST(rle)
{

    information::RLE::Encoder rle_encoder;

    size_t n=10;
    if(argc>1)
    {
        n = string_convert::to<size_t>(argv[1],"n");
    }
    string src(n+10,as_capacity,false);
    for(size_t i=1;i<=n;++i)
    {
        src += 'a';
    }
    std::cerr << "src=" << src << std::endl;
    rle_encoder.reset();
    const string out = rle_encoder.to_string(src);
    {
        ios::ocstream fp("rle.bin");
        fp << out;
    }


#if 0
    if(argc>1)
    {
        const string  fileName = argv[1];
        ios::icstream inp( fileName );
        ios::ocstream out( "rle.bin" );
        rle_encoder.process(out,inp);
    }
    else
    {
        for(size_t n=0;n<500;++n)
        {
            string src(n+10,as_capacity,false);
            for(size_t i=0;i<n;++i)
            {
                src += 'a';
            }
            rle_encoder.reset();
            const string out = rle_encoder.to_string(src);

        }
    }
#endif

}
Y_UTEST_DONE()


