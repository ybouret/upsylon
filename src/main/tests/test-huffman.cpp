#include "y/information/filter/huffman/encoder.hpp"
#include "y/information/filter/huffman/decoder.hpp"

#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/string.hpp"

using namespace upsylon;
using namespace information;

#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"
#include "y/hashing/sha1.hpp"

namespace {
    
    static inline
    digest testDigest( const string &backName, const size_t nmax )
    {
        hashing::sha1 H;
        H.set();
        {
            char C = 0;
            ios::icstream fp(backName);
            size_t n = 0;
            while( fp.query(C) )
            {
                if(++n>nmax) break;
                H(&C,1);
            }
            
        }
        return H.md();
        
    }
    
    void testHuffmanCodec(const Alphabet::Mode m,
                          const string        &fileName,
                          const string        &huffName,
                          const string        &backName)
    {
        size_t ni = 0;
        size_t no = 0;
        {
            Huffman::Encoder enc(m);
            ios::icstream    source( fileName );
            ios::ocstream    target( huffName );
            
            enc.reset();
            no = enc.process(target,source,&ni);
            std::cerr << "Encoder: " << ni << "->" << no << std::endl;
            enc.displayAlpha();
        }
        
        
        
        const digest h_ini = testDigest(fileName,ni);
        std::cerr << "h_ini=" << h_ini << std::endl;
        
        
        
        size_t nr = 0;
        size_t nd = 0;
        {
            Huffman::Decoder dec(m);
            ios::icstream    source( huffName );
            ios::ocstream    target( backName );
            
            dec.reset();
            nd = dec.process(target,source,&nr);
            std::cerr << "Decoder: " << nr << "->" << nd << std::endl;
        }
        const digest h_end= testDigest(fileName,ni);
        std::cerr << "h_end=" << h_end << std::endl;
        Y_CHECK(h_ini==h_end);
    }
    
}

Y_UTEST(huffman)
{
    
    std::cerr << "Alphabet::Chars = " << Alphabet::Chars << std::endl;
    std::cerr << "Alphabet::NYT   = " << Alphabet::NYT   << std::endl;
    std::cerr << "Alphabet::EOS   = " << Alphabet::EOS   << std::endl;
    std::cerr << "Alphabet::Codes = " << Alphabet::Codes << std::endl;
    std::cerr << "Alphabet::Nodes = " << Alphabet::Nodes << std::endl;
    
    Y_UTEST_SIZEOF(Alphabet::Node);
    
    
    if( argc> 1 )
    {
        const string  fileName = argv[1];
        {
            const string huffName = "shuff.bin";
            const string backName = "sback.bin";
            testHuffmanCodec(Alphabet::StreamMode, fileName, huffName, backName);
        }
        
        if(true)
        {
            const string huffName = "bhuff.bin";
            const string backName = "bback.bin";
            testHuffmanCodec(Alphabet::BufferMode, fileName, huffName, backName);
        }
        
    }
    else
    {
        hashing::sha1 H;
        H.set();
        Huffman::Encoder enc(Alphabet::StreamMode);
        size_t count = 0;
        {
            size_t u0 = 1;
            size_t u1 = 1;
            for(size_t ch=0;ch<=20;++ch)
            {
                const size_t n=u0+u1;
                for(size_t i=0;i<n;++i)
                {
                    const char C = char(ch);
                    enc.write(C);
                    H(&C,1);
                    ++count;
                 }
                u0=u1;
                u1=n;
            }
            enc.flush();
            enc.getRoot().graphViz("huff.dot");
        }
        std::cerr << "count    = " << count      << std::endl;
        std::cerr << "encoded  = " << enc.size() << std::endl;
        const digest h_ini = H.md();
        string  encoded( enc.size(), as_capacity, false);
        while( enc.size() )
        {
            encoded << enc.front(); enc.pop_front();
        }
        
        Huffman::Decoder dec(Alphabet::StreamMode);
        const string decoded = dec.to_string(encoded);
        std::cerr << "decoded: " << decoded.size() << std::endl;
        const digest h_end = H.md(decoded);
        std::cerr << h_ini << std::endl;
        std::cerr << h_end << std::endl;
        Y_CHECK(h_ini==h_end);
    }
    
    
    
    
    
    
}
Y_UTEST_DONE()

