#include "y/information/filter/huffman.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/string.hpp"

using namespace upsylon;
using namespace information;

Y_UTEST(huffman)
{

    std::cerr << "Alphabet::Chars = " << Alphabet::Chars << std::endl;
    std::cerr << "Alphabet::NYT   = " << Alphabet::NYT   << std::endl;
    std::cerr << "Alphabet::EOS   = " << Alphabet::EOS   << std::endl;
    std::cerr << "Alphabet::Codes = " << Alphabet::Codes << std::endl;

    Y_UTEST_SIZEOF(Alphabet::Node);
    std::cerr << "Huffman::Nodes = " << Huffman::Nodes << std::endl;

    information::Huffman sHuff( information::Alphabet::StreamMode );
    information::Huffman bHuff( information::Alphabet::BufferMode );

    sHuff.getRoot().graphViz("shuff.png");
    bHuff.getRoot().graphViz("bhuff.png");


}
Y_UTEST_DONE()

