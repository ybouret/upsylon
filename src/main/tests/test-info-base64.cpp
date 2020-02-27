#include "y/information/translator/any2base64.hpp"
#include "y/utest/run.hpp"

#include <cstring>

using namespace upsylon;
using namespace Information;

static inline void update_itable( unsigned itable[], const char *table )
{
    for(unsigned i=0;i<64;++i)
    {
        const uint8_t b = uint8_t(table[i]);
        if(itable[b]<64)
        {
            Y_ASSERT(itable[b] == i );
        }
        itable[b] = i;
    }
    
    itable[ uint8_t( Base64::padding ) ] = 64;
}

#include "y/ios/ocstream.hpp"
#include "y/ios/icstream.hpp"

Y_UTEST(base64)
{
    
    if( argc > 1 )
    {
        const string fileName = argv[1];
        ios::icstream fp( fileName );
        string line;
        while( fp.gets(line) )
        {
            {
                const size_t len = Base64::BytesFor(line.size(), Base64::Pad);
                string       b64(len,as_capacity,true);
                Base64::Encode(*b64, *line, line.size(), Base64::std_table, Base64::Pad);
                std::cerr << "\t" << b64 << std::endl;
            }
            
            {
                const size_t len = Base64::BytesFor(line.size(), Base64::Raw);
                string       b64(len,as_capacity,true);
                Base64::Encode(*b64, *line, line.size(), Base64::std_table, Base64::Raw);
                std::cerr << "\t" << b64 << std::endl;
            }
        }
    }
    else
    {
        std::cerr << "-- Base64: check tables" << std::endl;
        unsigned itable[256] = { 0 };
        for(size_t i=0;i<256;++i) itable[i] = 128;
        update_itable(itable, Base64::std_table );
        update_itable(itable, Base64::url_table );
        
        if(false)
        {
            ios::ocstream fp("base64-itable.inc");
            for(size_t i=0;i<256;++i)
            {
                fp("%3u", itable[i]);
                if(i<255) fp << ',';
                if( 0==((i+1) % 16) ) fp << '\n';
            }
        }
        
        std::cerr << "-- Base64: check encode/decode" << std::endl;
        char    output[4] = { 0,0,0,0,};
        uint8_t input[3]  = { 0 };
        uint8_t check[3]  = { 0 };
        size_t  count1 = 0;
        size_t  count2 = 0;
        size_t  count3 = 0;
#define __CLR() do { memset(output,0,sizeof(output)); memset(check,0,sizeof(check)); } while(false)
        
        for(size_t i=0;i<256;++i)
        {
            input[0] = uint8_t(i);
            
            {
                __CLR();
                Base64::Encode1to2(output, input, Base64::std_table);
                Base64::Decode2to1(check,output);
                Y_ASSERT( (input[0] == check[0]) || die("std") );
                __CLR();
                Base64::Encode1to2(output, input, Base64::url_table);
                Base64::Decode2to1(check,output);
                Y_ASSERT( (input[0] == check[0]) || die("url") );
                ++count1;
            }
            
            for(size_t j=0;j<256;++j)
            {
                input[1] = uint8_t(j);
                
                {
                    __CLR();
                    Base64::Encode2to3(output, input, Base64::std_table);
                    Base64::Decode3to2(check,output);
                    Y_ASSERT(input[0] == check[0] );
                    Y_ASSERT(input[1] == check[1] );
                    
                    __CLR();
                    Base64::Encode2to3(output, input, Base64::url_table);
                    Base64::Decode3to2(check,output);
                    Y_ASSERT(input[0] == check[0] );
                    Y_ASSERT(input[1] == check[1] );
                    ++count2;
                }
                
                for(size_t k=0;k<256;++k)
                {
                    input[2] = uint8_t(k);
                    
                    {
                        __CLR();
                        Base64::Encode3to4(output, input, Base64::std_table );
                        Base64::Decode4to3(check,output);
                        Y_ASSERT(input[0] == check[0] );
                        Y_ASSERT(input[1] == check[1] );
                        Y_ASSERT(input[2] == check[2] );
                        
                        __CLR();
                        Base64::Encode3to4(output, input, Base64::url_table );
                        Base64::Decode4to3(check,output);
                        Y_ASSERT(input[0] == check[0] );
                        Y_ASSERT(input[1] == check[1] );
                        Y_ASSERT(input[2] == check[2] );
                        
                        ++count3;
                    }
                }
            }
        }
        std::cerr << "#single=" << count1 << std::endl;
        std::cerr << "#double=" << count2 << std::endl;
        std::cerr << "#triple=" << count3 << std::endl;
        
        std::cerr << "-- Base64: check length" << std::endl;
        for(size_t len=0;len<=32000;++len)
        {
            const size_t bytes_for_raw = Base64::BytesFor(len,Base64::Raw);
            const size_t bytes_for_pad = Base64::BytesFor(len,Base64::Pad);
            
            //std::cerr << "length=" << len << " -> " << bytes_for_raw << " / " << bytes_for_pad << std::endl;
            Y_ASSERT( bytes_for_raw == Y_BASE64_RAW_BYTES_FOR(len) );
            Y_ASSERT( bytes_for_pad == Y_BASE64_PAD_BYTES_FOR(len) );
        }
        
    }
    
    
}
Y_UTEST_DONE()

