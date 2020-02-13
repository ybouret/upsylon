#include "y/information/filter/huffman.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/string.hpp"

using namespace upsylon;
using namespace information;

#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"

namespace {

    void testHuffmanCodec(const Alphabet::Mode m,
                          const string        &fileName,
                          const string        &huffName,
                          const string        &backName)
    {
        size_t ni = 0;
        size_t no = 0;
        {
            Huffman::Encoder enc(m);
            ios::icstream source( fileName );
            ios::ocstream target( huffName );

            enc.reset();
            no = enc.process(target,source,&ni);
            std::cerr << "Encoder: " << ni << "->" << no << std::endl;
        }

        size_t nr = 0;
        size_t nd = 0;
        {
            Huffman::Decoder dec(m);
            ios::icstream source( huffName );
            ios::ocstream target( backName );

            dec.reset();
            nd = dec.process(target,source,&nr);
            std::cerr << "Decoder: " << nr << "->" << nd << std::endl;
        }
    }

}

Y_UTEST(huffman)
{

    std::cerr << "Alphabet::Chars = " << Alphabet::Chars << std::endl;
    std::cerr << "Alphabet::NYT   = " << Alphabet::NYT   << std::endl;
    std::cerr << "Alphabet::EOS   = " << Alphabet::EOS   << std::endl;
    std::cerr << "Alphabet::Codes = " << Alphabet::Codes << std::endl;
    std::cerr << "Alphabet::Nodes = " << Alphabet::Nodes << std::endl;

    Y_UTEST_SIZEOF(Alphabet::Node);


    if( argc> 1 )
    {
        const string  fileName = argv[1];
        {
            const string huffName = "shuff.bin";
            const string backName = "sback.bin";
            testHuffmanCodec(Alphabet::StreamMode, fileName, huffName, backName);
        }

        {
            const string huffName = "bhuff.bin";
            const string backName = "bback.bin";
            testHuffmanCodec(Alphabet::BufferMode, fileName, huffName, backName);
        }


#if 0
        {
            size_t nis = 0;
            ios::icstream source( fileName );
            ios::ocstream target( "shuff.bin" );
            sHuff.reset();
            const size_t nos = sHuff.process(target,source,&nis);
            std::cerr << nis << "->" << nos << std::endl;
        }

        if(true)
        {
            size_t nib = 0;
            ios::icstream source( fileName );
            ios::ocstream target( "bhuff.bin" );
            bHuff.reset();
            const size_t nob = bHuff.process(target,source,&nib);
            std::cerr << nib << "->" << nob << std::endl;
        }
#endif

    }






    
}
Y_UTEST_DONE()

