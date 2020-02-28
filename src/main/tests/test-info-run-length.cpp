#include "y/information/translator/run-length/encoder.hpp"
#include "y/information/translator/shannon-fano/encoder.hpp"
#include "y/information/translator/huffman/encoder.hpp"

#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"
#include "y/string.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;
using namespace Information;

Y_UTEST(run_length)
{
    typedef RunLength::Single<QEncoder,QWriter> I1;
    RunLength::EncodeWith<I1> encI1;

    typedef RunLength::Single<QEncoder,ShannonFano::EncodingTree> S1;
    RunLength::EncodeWith<S1> encS1;

    typedef RunLength::Double<QEncoder,ShannonFano::EncodingTree,ShannonFano::EncodingTree> S2;
    RunLength::EncodeWith<S2> encS2;

    typedef RunLength::Triple<QEncoder,ShannonFano::EncodingTree,ShannonFano::EncodingTree,ShannonFano::EncodingTree> S3;
    RunLength::EncodeWith<S3> encS3;

    typedef RunLength::Triple<QEncoder,Huffman::EncodingTree,Huffman::EncodingTree,Huffman::EncodingTree> H3;
    RunLength::EncodeWith<H3> encH3;

    Translator    *tr[] = { &encI1, &encS1, &encS2, &encS3, &encH3};
    const size_t   nt   = sizeof(tr)/sizeof(tr[0]);
    vector<size_t> encoded(nt,0);

    
    if( argc > 1 )
    {
        const string  fileName = argv[1];
        ios::icstream input( fileName );

        for(size_t i=0;i<nt;++i) tr[i]->reset();
        char C = 0;
        size_t count = 0;
        while( input.query(C) )
        {
            ++count;
            for(size_t i=0,j=1;i<nt;++i,++j)
            {
                Translator &t = *tr[i];
                t.write(C);

                char E = 0;
                while( t.query(E) )
                {
                    ++encoded[j];
                }
            }
        }

        for(size_t i=0,j=1;i<nt;++i,++j)
        {
            Translator &t = *tr[i];
            t.flush();

            char E = 0;
            while( t.query(E) )
            {
                ++encoded[j];
            }
        }
        std::cerr << "encoded " << count << " to " << encoded << std::endl;
    }




}
Y_UTEST_DONE()

