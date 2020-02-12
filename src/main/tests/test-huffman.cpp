#include "y/information/filter/huffman.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/string.hpp"

using namespace upsylon;
using namespace information;

#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"

Y_UTEST(huffman)
{

    std::cerr << "Alphabet::Chars = " << Alphabet::Chars << std::endl;
    std::cerr << "Alphabet::NYT   = " << Alphabet::NYT   << std::endl;
    std::cerr << "Alphabet::EOS   = " << Alphabet::EOS   << std::endl;
    std::cerr << "Alphabet::Codes = " << Alphabet::Codes << std::endl;
    std::cerr << "Alphabet::Nodes = " << Alphabet::Nodes << std::endl;

    Y_UTEST_SIZEOF(Alphabet::Node);

    Huffman::Encoder sHuff( Alphabet::StreamMode );
    Huffman::Encoder bHuff( Alphabet::BufferMode );

    sHuff.displayAlpha();
    bHuff.displayAlpha();

    if( argc> 1 )
    {
        const string  fileName = argv[1];
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

    }


    sHuff.displayAlpha();
    bHuff.displayAlpha();
    sHuff.getRoot().graphViz("stree.dot");
    bHuff.getRoot().graphViz("btree.dot");




    
}
Y_UTEST_DONE()

