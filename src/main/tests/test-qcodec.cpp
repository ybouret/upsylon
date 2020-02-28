#include "y/information/translator/huffman/encoder.hpp"
#include "y/information/translator/shannon-fano/encoder.hpp"
#include "y/utest/run.hpp"

#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"
#include "y/string.hpp"

using namespace upsylon;
using namespace Information;

static inline void saveQ(const string &compName,
                         IOBits       &Q,
                         list<char>   &S)
{
    ios::ocstream fp(compName);
    while( Q.size() >= 8 )
    {
        const char C = Q.pop<char>();
        Y_ASSERT( C == S.front() );
        fp.write( C );
        S.pop_front();
    }
    std::cerr << "<Same Outputs>" << std::endl;
}

Y_UTEST(qcodec)
{

    QEncoder::Pointer HuffmanEnc     = new  Huffman::EncodingTree();
    QEncoder::Pointer ShannonFanoEnc = new  ShannonFano::EncodingTree();

    IOBits HuffmanQ;
    IOBits ShannonFanoQ;

    Huffman::Encoder     HuffmanEncoder;
    ShannonFano::Encoder ShannonFanoEncoder;


    if( argc > 1 )
    {
        const string fileName = argv[1];
        HuffmanEnc->startBits();
        ShannonFanoEnc->startBits();
        HuffmanEncoder.reset();
        {
            ios::icstream source( fileName );
            char C=0;
            while( source.query(C) )
            {
                HuffmanEnc->writeBits(HuffmanQ,C);
                ShannonFanoEnc->writeBits(ShannonFanoQ,C);
                HuffmanEncoder.write(C);
                ShannonFanoEncoder.write(C);
            }
            HuffmanEnc->flushBits(HuffmanQ);
            ShannonFanoEnc->flushBits(ShannonFanoQ);

            HuffmanEncoder.flush();
            ShannonFanoEncoder.flush();
        }
        std::cerr << "HuffmanQ     : " << HuffmanQ.size() << " bits" << std::endl;
        std::cerr << "ShannonFanoQ : " << ShannonFanoQ.size() << " bits" << std::endl;
        Y_CHECK( 0 == (HuffmanQ.size()     & 7) );
        Y_CHECK( 0 == (ShannonFanoQ.size() & 7) );
        Y_CHECK( HuffmanEncoder.size() == HuffmanQ.size()/8 );
        Y_CHECK( ShannonFanoEncoder.size() == ShannonFanoQ.size()/8 );


        const string HuffmanComp     = "huffman-raw.bin";
        const string ShannonFanoComp = "shannon-fano-raw.bin";
        saveQ(HuffmanComp,    HuffmanQ,     HuffmanEncoder);
        saveQ(ShannonFanoComp,ShannonFanoQ, ShannonFanoEncoder);



    }



}
Y_UTEST_DONE()
