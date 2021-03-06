#include "y/information/translator/huffman/encoder.hpp"
#include "y/information/translator/huffman/decoder.hpp"

#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/string.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"

#include "y/fs/disk/file.hpp"
#include "y/hashing/sha1.hpp"

using namespace upsylon;
using namespace Information;



Y_UTEST(huffman)
{
    Huffman::Encoder enc;
    Huffman::Decoder dec;
    string           fileName = "fibonacci.bin";
    const string     compName = "huffman.bin";
    const string     backName = "huffman_org.bin";

    if( argc > 1 )
    {
        fileName = argv[1];
    }
    else
    {
        (void) Translator::Fibonacci(fileName, 'a', 'z' );
    }
    enc.testCODEC(fileName, compName, backName, &dec);
    

}
Y_UTEST_DONE()

