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
using namespace information;

namespace {
    
    static inline void testRepetitions()
    {
        RLE::Encoder rle;
        
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
    
   
    
}

Y_UTEST(rle)
{
    std::cerr << "RLE::MaxRepeating=" << int(information::RLE::Parameters::MaxRepeating) << std::endl;
    std::cerr << "RLE::MaxDifferent=" << int(information::RLE::Parameters::MaxDifferent) << std::endl;
    
    if(argc>1)
    {
        const string fileName = argv[1];
        {
            const string compName = "rle.bin";
            const string backName = "unrle.bin";
            RLE::Encoder rle;
            RLE::Decoder unrle;
            rle.testCODEC(fileName, compName, backName, &unrle);
        }
        
        if( fileName != Y_STDIN )
        {
            const string compName = "rle1.bin";
            const string backName = "unrle1.bin";

            RunLength::EncodeWith<  RunLength::SingleEncoder<qwriter> > enc1;
            enc1.testCODEC(fileName, compName, backName, NULL);


        }
    }
    else
    {
        testRepetitions();
        testRandom();
    }
    
}
Y_UTEST_DONE()


