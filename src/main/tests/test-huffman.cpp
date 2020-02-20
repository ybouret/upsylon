//#include "y/information/filter/huffman/encoder.hpp"
//#include "y/information/filter/huffman/decoder.hpp"

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
using namespace information;



Y_UTEST(huffman)
{
    Y_UTEST_SIZEOF(Alphabet);
    Y_UTEST_SIZEOF(Alphabet::Char);

    
    Huffman::Encoder enc;
    Huffman::Decoder dec;
    string           fileName;
    const string     compName = "huff.bin";
    const string     backName = "horg.bin";

    if(argc>1)
    {
        fileName = argv[1];
        size_t nr = 0;
        size_t nc = 0;
        {
            ios::icstream source( fileName );
            ios::ocstream target( compName );
            enc.reset();
            nc = enc.process(target, source, &nr );
        }
        std::cerr << "huff : " << nr << " -> " << nc << std::endl;
        size_t ns = 0;
        size_t nd = 0;
        {
            ios::icstream source( compName );
            ios::ocstream target( backName );
            dec.reset();
            nd = dec.process(target, source, &ns );
        }
        std::cerr << "back : " << ns << " -> " << nd << std::endl;
        Y_CHECK( nd == nr );
    }
    else
    {
        std::cerr << "<compression>" << std::endl;
        fileName    = "fibo.bin";
        size_t ngen = 0;
        size_t nenc = enc.testFibonnaci(compName,fileName, 'a', 'a'+20, ngen);
        std::cerr << "fibonacci: " << ngen << " -> " << nenc << std::endl;

        std::cerr << "<decompression>" << std::endl;
        size_t ns = 0;
        size_t nd = 0;
        {
            ios::icstream source( compName );
            ios::ocstream target( backName );
            dec.reset();
            nd = dec.process(target, source, &ns );
        }
        std::cerr << "back : " << ns << " -> " << nd << std::endl;
        Y_CHECK( nd == ngen );
    }
    hashing::sha1 H;
    const digest hashOrg = ios::disk_file::md(H,fileName);
    const digest hashEnd = ios::disk_file::md(H,backName);
    Y_CHECK( hashEnd == hashOrg );



}
Y_UTEST_DONE()

