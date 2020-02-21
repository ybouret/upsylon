#include "y/utest/run.hpp"
#include "y/information/bwt.hpp"
#include "y/fs/disk/file.hpp"
#include "y/string.hpp"
#include "y/sequence/vector.hpp"
#include "y/information/modulation/mtf.hpp"
#include "y/string/convert.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/imstream.hpp"

#include "y/information/translator/huffman/encoder.hpp"
#include "y/information/translator/shannon-fano/encoder.hpp"

using namespace upsylon;
using namespace information;

Y_UTEST(pack)
{

    string input;
    if(argc>1)
    {
        const string fileName = argv[1];
        const size_t nr = ios::disk_file::load(input,fileName);
        Y_CHECK(nr==input.size());
    }
    const size_t         sz = input.size();
    string               output(sz, as_capacity, true );
    vector<size_t>       indices(sz,0);
    mtf_encoder          mtf;
    bwt::encode(*output, *input, sz, *indices,mtf);
    {
        ios::ocstream fp("ipack.bin"); fp << input;
    }

    {
        ios::ocstream fp("opack.bin"); fp << output;
    }

    size_t nHuffman = 0;
    {
        ios::imstream source( output );
        ios::ocstream target( "pack-huffman.bin" );
        Huffman::Encoder enc;
        nHuffman = enc.process(target,source);
    }
    std::cerr << " huffman     : " << sz << " -> " << nHuffman     << std::endl;

    size_t nShannonFano = 0;
    {
        ios::imstream source( output );
        ios::ocstream target( "pack-shannon-fano.bin" );
        ShannonFano::Encoder enc;
        nShannonFano = enc.process(target,source);
    }
    std::cerr << " shannon-fano: " << sz << " -> " << nShannonFano << std::endl;


#if 0
    information::Pack::Encoder   packEnc(1024*512);

    if(argc>1)
    {
        const string   fileName = argv[1];
        ios::icstream  source( fileName );
        ios::ocstream  target( "pack.bin" );

        packEnc.reset();
        size_t       nr = 0;
        const size_t nc = packEnc.process(target, source, &nr);
        std::cerr << "packEnc: " << nr << " -> " << nc << std::endl;

    }
#endif

}
Y_UTEST_DONE()
