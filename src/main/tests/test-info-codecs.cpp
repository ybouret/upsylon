#include "y/information/translator/qrl/encoder.hpp"
#include "y/information/translator/qrl/decoder.hpp"

#include "y/information/translator/huffman/encoder.hpp"
#include "y/information/translator/huffman/decoder.hpp"

#include "y/information/translator/shannon-fano/encoder.hpp"
#include "y/information/translator/shannon-fano/decoder.hpp"

#include "y/information/translator/base64/encoder.hpp"
#include "y/information/translator/base64/decoder.hpp"

#include "y/sequence/vector.hpp"
#include "y/ios/imstream.hpp"
#include "y/ios/ocstream.hpp"

#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Information;

namespace {

    static inline
    size_t testCoDec(Translator        &enc,
                     Translator        &dec,
                     const array<char> &src)
    {

        vector<char>  target( 1024*1024, as_capacity);
        size_t        nInp = 0;
        size_t        nEnc = 0;

        enc.reset();
        {
            ios::imstream source( *src, src.size() );
            //ios::ocstream inp( "source.bin" );
            //ios::ocstream out( "codecs.bin" );
            char C = 0;
            while( source.query(C) )
            {
                ++nInp;
                //inp << C;
                enc.write(C);
                while( enc.query(C) )
                {
                    target << C;
                    ++nEnc;
                    //out << C;
                }
            }
            enc.flush();
            while( enc.query(C) )
            {
                target << C;
                ++nEnc;
                //out << C;
            }
        }
        Y_ASSERT(nInp==src.size());


        dec.reset();
        size_t nDec = 0;
        {
            char C = 0;
            for(size_t i=1; i<= nEnc; ++i)
            {
                dec.write(target[i]);
                while( dec.query(C) )
                {
                    ++nDec;
                    Y_ASSERT(nDec<=nInp);
                    Y_ASSERT(src[nDec]==C);
                }
            }
            dec.flush();
            while( dec.query(C) )
            {
                ++nDec;
                Y_ASSERT(nDec<=nInp);
                Y_ASSERT(src[nDec]==C);
            }
        }
        Y_ASSERT( nDec == nInp );
        return nEnc;
    }

    static inline void fullCoDec(Translator   &enc,
                                 Translator   &dec)
    {
        static const size_t maxLength = 16*1024;
        vector<char> data( 16*maxLength, as_capacity );
        std::cerr << "Encoding with " << enc.family() << " " << enc.name() << std::endl;
        std::cerr << "Decoding with " << dec.family() << " " << dec.name() << std::endl;

        {
            std::cerr << "blocks    : [";
            size_t count = 0;
            for(size_t n=0;n<=maxLength;)
            {
                for(size_t iter=1;iter<=4;++iter)
                {
                    data.free();
                    while( data.size() < n )
                    {
                        const size_t left  = n-data.size();
                        const size_t todo  = alea.leq(left);
                        const char   C     = alea.range('a', 'z');
                        for(size_t i=0;i<todo;++i)
                        {
                            data << C;
                        }
                    }
                    testCoDec(enc,dec,data);
                }
                if(n<=1024)
                {
                    ++n;
                }
                else
                {
                    n += 1024;
                }
                ++count;
                if( 0 == (count%16) )
                {
                    std::cerr << ".";
                    std::cerr.flush();
                }
            }
            std::cerr << "]" << std::endl;
        }

        {
            data.free();
            size_t u0 = 0;
            size_t u1 = 1;
            for( char i='a'; i <= 'a'+23; ++i )
            {
                size_t u2 = u0+u1;
                for(size_t j=0;j<u2;++j)
                {
                    data << i;
                }
                u0 = u1;
                u1 = u2;
            }
            std::cerr << "fibonacci : [ " << data.size() << "]" << std::endl;
            const size_t nEnc = testCoDec(enc,dec,data);
            std::cerr << "            |_" << nEnc << std::endl;
        }
    }

}

Y_UTEST(codecs)
{



    {
        Base64Encoder enc;
        Base64Decoder dec;
        fullCoDec(enc,dec);
    }

    {
        Base64Encoder enc(Base64::Raw);
        Base64Decoder dec;
        fullCoDec(enc,dec);
    }
    
    {
        QRL::Encoder enc;
        QRL::Decoder dec;
        fullCoDec(enc,dec);
    }



    {
        Huffman::Encoder enc;
        Huffman::Decoder dec;
        fullCoDec(enc,dec);
    }


    {
        ShannonFano::Encoder enc;
        ShannonFano::Decoder dec;
        fullCoDec(enc,dec);
    }
}
Y_UTEST_DONE()

