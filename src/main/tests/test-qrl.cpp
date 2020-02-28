#include "y/information/translator/qrl/encoder.hpp"
#include "y/information/translator/qrl/decoder.hpp"

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
using namespace Information;

namespace {
    
    static inline void testRepetitions()
    {
        QRL::Encoder rle;
        
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
        QRL::Encoder rle;
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

Y_UTEST(qrl)
{
    std::cerr << "QRL::MaxRepeating=" << int(QRL::Parameters::MaxRepeating) << std::endl;
    std::cerr << "QRL::MaxDifferent=" << int(QRL::Parameters::MaxDifferent) << std::endl;
    
    if(argc>1)
    {
        const string fileName = argv[1];
        {
            const string compName = "rle.bin";
            const string backName = "unrle.bin";
            QRL::Encoder rle;
            QRL::Decoder unrle;
            rle.testCODEC(fileName, compName, backName, &unrle);
        }
        
        if( fileName != Y_STDIN )
        {
            const string compName = "rle1.bin";
            const string backName = "unrle1.bin";

            RunLength::EncodeWith< RunLength::Single<QEncoder,QWriter> >         enc1;
            enc1.testCODEC(fileName, compName, backName, NULL);
        }

        if( fileName != Y_STDIN )
        {
            const string compName = "rle2.bin";
            const string backName = "unrle2.bin";

            RunLength::EncodeWith< RunLength::Double<QEncoder,QWriter,QWriter> > enc2;
            enc2.testCODEC(fileName, compName, backName, NULL);
        }

        if( fileName != Y_STDIN )
        {
            const string compName = "rle3.bin";
            const string backName = "unrle3.bin";

            RunLength::EncodeWith< RunLength::Triple<QEncoder,QWriter,QWriter,QWriter> > enc3;
            enc3.testCODEC(fileName, compName, backName, NULL);
        }



    }
    else
    {
        testRepetitions();
        testRandom();
    }
    
}
Y_UTEST_DONE()


