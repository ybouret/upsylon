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
        const string backName = "horg.bin";
        size_t ns = 0;
        size_t nd = 0;
        {
            ios::icstream source( compName );
            ios::ocstream target( backName );
            nd = dec.process(target, source, &ns );
        }
        std::cerr << "back : " << ns << " -> " << nd << std::endl;
        Y_CHECK( nd == nr );
        hashing::sha1 H;
        const digest hashOrg = ios::disk_file::md(H,fileName);
        const digest hashEnd = ios::disk_file::md(H,backName);
        Y_CHECK( hashEnd == hashOrg );
    }




}
Y_UTEST_DONE()

