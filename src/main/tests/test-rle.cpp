#include "y/information/filter/rle/encoder.hpp"
#include "y/information/filter/rle/decoder.hpp"
#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"
#include "y/string.hpp"
#include "y/string/convert.hpp"

using namespace upsylon;

namespace {

    static inline void testRep()
    {
        std::cerr << "<RLE_Repetitions>" << std::endl;
        const size_t nmax = 1000;
        information::RLE::Encoder enc;
        information::RLE::Decoder dec;
        string src(nmax,as_capacity,false);

        for(size_t n=0;n<nmax;++n)
        {
            src.clear();
            const char C = alea.range<char>('a','z');
            for(size_t i=0;i<n;++i)
            {
                src << C;
            }
            enc.reset();
            const string encoded = enc.to_string(src);

            dec.reset();
            const string decoded = dec.to_string(encoded);
            Y_ASSERT( decoded == src );
        }
        std::cerr << "<RLE_Repetitions/>" << std::endl << std::endl;


    }

}

Y_UTEST(rle)
{
    testRep();

    information::RLE::Encoder enc;
    information::RLE::Decoder dec;

    if(argc>1)
    {
        const string fileName = argv[1];
        const string compName = "rle.bin";
        const string backName = "unrle.bin";
        size_t nr = 0;
        size_t nc = 0;
        {
            ios::icstream source( fileName );
            ios::ocstream target( compName );
            enc.reset();
            nc = enc.process(target,source, &nr);
        }
        std::cerr << "rle  : " << nr << " -> " << nc << std::endl;

        size_t nd = 0;
        size_t ns = 0;
        {
            ios::icstream source( compName );
            ios::ocstream target( backName );
            dec.reset();
            nd = dec.process(target, source, &ns );
        }
        std::cerr << "unrle : " << ns << " -> " << nd << std::endl;
        Y_CHECK( nd == nr );
        Y_CHECK( ns == nc );
    }



#if 0
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
#endif


}
Y_UTEST_DONE()


