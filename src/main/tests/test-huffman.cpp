//#include "y/information/filter/huffman/encoder.hpp"
//#include "y/information/filter/huffman/decoder.hpp"

#include "y/information/translator/huffman/encoder.hpp"

#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/string.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace information;



Y_UTEST(huffman)
{
    Y_UTEST_SIZEOF(Alphabet);
    Y_UTEST_SIZEOF(Alphabet::Char);

#if 0
    Alphabet alpha;
    Alphabet alpha2;

    alpha.displayChars();
    qbits io;
    alpha.updateByte(0,&io);
    alpha2.updateByte(0,NULL);
    alpha.displayChars();
    Y_CHECK( alpha.sameThan(alpha2) );
    for(size_t i=1;i<256;++i)
    {
        alpha.updateByte(i,&io);
        alpha2.updateByte(i,NULL);
        io.free();
    }
    alpha.displayChars();
    Y_CHECK(alpha.sameThan(alpha2));

    std::cerr << "Testing..." << std::endl;
    for(size_t iter=0;iter<16;++iter)
    {
        alpha.initialize();
        alpha2.initialize();
        Y_ASSERT(alpha.sameThan(alpha2));
        for(size_t nch=1000+alea.leq(1000);nch>0;--nch)
        {
            const uint8_t ch = alea.full<uint8_t>();
            (void) alpha.updateByte(ch,&io); io.free();
            (void) alpha2.updateByte(ch,NULL);
            Y_ASSERT(alpha.sameThan(alpha2));
        }
    }
#endif
    
    Huffman::Encoder enc;

    if(argc>1)
    {
        const string  fileName = argv[1];
        const string  compName = "huff.bin";
        size_t nr = 0;
        size_t nc = 0;
        {
            ios::icstream source( fileName );
            ios::ocstream target( compName );
            nc = enc.process(target, source, &nr );
        }
        std::cerr << "huff : " << nr << " -> " << nc << std::endl;
    }


    enc.displayChars();
    enc.getRoot().graphViz("huff.dot");

}
Y_UTEST_DONE()

