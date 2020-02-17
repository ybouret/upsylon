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

    static inline void testReps()
    {
        size_t count =0;
        {
            ios::ocstream source( "rle-source.bin" );
            char C = 'a';
            for(size_t blocks=5+alea.leq(10);blocks>0;--blocks)
            {
                ++C;
                for(size_t n=alea.leq(1000);n>0;--n)
                {
                    source << C;
                    ++count;
                }
            }
        }
        
        size_t n_src=0;
        size_t n_tgt=0;
        {
            ios::icstream             source( "rle-source.bin" );
            ios::ocstream             target( "rle-target.bin" );
            information::RLE::Encoder enc;
            enc.reset();
            n_tgt = enc.process(target,source,&n_src);
        }
        Y_CHECK(count==n_src);
        
        size_t n_dec=0;
        size_t n_enc=0;
        {
            ios::icstream             source( "rle-target.bin" );
            ios::ocstream             target( "rle-recovr.bin" );
            information::RLE::Decoder dec;
            dec.reset();
            n_dec = dec.process(target,source,&n_enc);
        }
        Y_CHECK(n_enc==n_tgt);
    }
}

Y_UTEST(rle)
{

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
    else
    {
        testRep();
        testReps();
    }

}
Y_UTEST_DONE()


