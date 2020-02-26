#include "y/information/translator/rle/encoder.hpp"
#include "y/information/translator/rle/decoder.hpp"

#include "y/information/translator/run-length/encoder.hpp"

#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"
#include "y/string.hpp"
#include "y/string/convert.hpp"
#include "y/fs/disk/file.hpp"
#include "y/hashing/md5.hpp"
#include "y/ios/null-ostream.hpp"

using namespace upsylon;

namespace {

    static inline void testRepetitions()
    {
        information::RLE::Encoder rle;

        for(size_t blockSize=0;blockSize<= 1000; ++blockSize )
        {
            std::cerr << "<RLE.BlockSize=" << blockSize << ">" << std::endl;
            rle.reset();
            const char C = alea.range<char>('a','z');
            for(size_t i=0;i<blockSize;++i)
            {
                rle.write(C);
            }
            rle.flush();
            std::cerr << "<RLE.BlockSize=" << blockSize << "/>" << std::endl << std::endl;

        }
    }

    static inline void testRandom()
    {
        information::RLE::Encoder rle;
        size_t count = 0;
        while( count < 1000 )
        {
            const char   C  = alea.range<char>('a','z');
            const size_t sz = alea.leq(100);
            rle.repeat(sz,C);
            count += sz;
        }
        rle.flush();
    }

    static inline void testComparison()
    {
        information::RunLength::EncodeWith< information::RunLength::SingleEncoder<information::qwriter> > enc1;
        
    }
    
    
}

Y_UTEST(rle)
{
    std::cerr << "RLE::MaxRepeating=" << int(information::RLE::Parameters::MaxRepeating) << std::endl;
    std::cerr << "RLE::MaxDifferent=" << int(information::RLE::Parameters::MaxDifferent) << std::endl;

    if(argc>1)
    {
        const string              fileName = argv[1];
        const string              compName = "rle.bin";
        const string              backName = "unrle.bin";
        information::RLE::Encoder rle;
        information::RLE::Decoder unrle;

        rle.testCODEC(fileName, compName, backName, &unrle);
    }
    else
    {
        testRepetitions();
        testRandom();
    }

}
Y_UTEST_DONE()


