#include "y/codec/asf.hpp"
#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;

Y_UTEST(asf)
{
    ASF::Encoder  enc;

#if 0
    ASF::Alphabet alpha;
    alpha.display(std::cerr);
    //alpha.GraphViz("asf0.dot");
    iobits io;

    size_t ibytes=0;
    size_t obytes=0;
#endif
    if( argc>1 && 0 == strcmp("run",argv[1]))
    {
        ios::icstream fp( ios::cstdin );
        ios::ocstream os( "asf.bin" );
        enc.filter(os,fp);
        
#if 0
        char C = 0;
        while( fp.query(C) )
        {
            ++ibytes;
            alpha.encode(io,C);
            while(io.size>=8)
            {
                os.write( io.pop_full<uint8_t>() );
                ++obytes;
                if(0==(obytes%16384))
                {
                    std::cerr << '.'; std::cerr.flush();
                }
            }

        }
        alpha.encode_eos(io); assert(0==io.size%8);
        while(io.size>=8)
        {
            os.write( io.pop_full<uint8_t>() );
            ++obytes;
        }
        std::cerr << "bytes: " << ibytes << " => " << obytes << std::endl;
        if(ibytes>0)
        {
            const double ratio = (100.0 * obytes)/ibytes;
            std::cerr << "is " << ratio << "% of original" << std::endl;
        }
#endif
    }


}
Y_UTEST_DONE()

