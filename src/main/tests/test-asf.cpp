#include "y/codec/asf.hpp"
#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"
#include "y/sequence/vector.hpp"
#include "y/ios/imstream.hpp"
#include "y/ios/osstream.hpp"
#include "y/hashing/sha1.hpp"

using namespace upsylon;

Y_UTEST(asf)
{
    ASF::Encoder  enc;
    ASF::Decoder  dec;



    if( argc>1 && 0 == strcmp("run",argv[1]))
    {
        enc.reset();
        {
            ios::icstream fp( ios::cstdin );
            ios::ocstream os( "asf_encoded.bin" );
            enc.filter(os,fp);
        }
        dec.reset();
        {
            ios::icstream fp( "asf_encoded.bin" );
            ios::ocstream os( "asf_decoded.bin" );
            dec.filter(os,fp);
        }
    }
    else
    {
        hashing::sha1 H;
        string source;
        string target;
        string decode;
        for(size_t iter=0;iter<8;++iter)
        {
            source.clear();
            for(size_t j=1+alea.leq(4);j>0;--j)
            {
                for(size_t i=0;i<256;++i)
                {
                    source << char(i);
                }
            }

            alea.shuffle(*source,source.size());
            const uint64_t H0 = H.key<uint64_t>(source);
            enc.reset();
            target.clear();
            {
                ios::imstream input(source);
                ios::osstream output(target);
                enc.filter(output,input);
            }

            std::cerr << source.size() << " -> " << target.size() << "/" << H0 << std::endl;
            dec.reset();
            decode.clear();
            {
                ios::imstream input(target);
                ios::osstream output(decode);
                dec.filter(output,input);
            }
            const uint64_t H1 = H.key<uint64_t>(decode);
            std::cerr << target.size() << " -> " << decode.size() << "/" << H1 << std::endl;
            Y_ASSERT(H0==H1);
        }
    }

}
Y_UTEST_DONE()

