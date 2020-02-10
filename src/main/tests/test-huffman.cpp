#include "y/information/huffman.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;
using namespace information;

Y_UTEST(huffman)
{
    std::cerr << "Huffman::AlphabetSize = " << Huffman::AlphabetSize << std::endl;
    std::cerr << "Huffman::NumNodes     = " << Huffman::NumNodes     << std::endl;

    Y_UTEST_SIZEOF(Huffman::Node);
    Y_UTEST_SIZEOF(Huffman::Tree);

    Huffman::Tree stree( Huffman::StreamMode );
    Huffman::Tree btree( Huffman::BufferMode );

    stree.getRoot().graphViz("stree.dot");
    btree.getRoot().graphViz("btree.dot");


}
Y_UTEST_DONE()

