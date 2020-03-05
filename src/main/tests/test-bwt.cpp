#include "y/information/bwt.hpp"
#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"
#include "y/sequence/vector.hpp"
#include "y/string.hpp"

#include "y/information/modulation/delta.hpp"
#include "y/information/modulation/mtf.hpp"

#include "y/string/convert.hpp"

using namespace upsylon;
using namespace Information;

#include "y/information/modulation/echo.hpp"
#include "y/information/entropy.hpp"
#include "y/ptr/auto.hpp"

static inline
void do_bwt(const string &input)
{
    const size_t   n = input.size();
    string         output(n,as_capacity,true);
    string         outputEcho(n,as_capacity,true);
    string         outputDelta(n,as_capacity,true);
    string         outputMTF(n,as_capacity,true);
    string         decoded(n,as_capacity,true);

    Entropy S;
    std::cerr << "$input       : " << S.of(input)       << std::endl;


    vector<size_t> indices(n,0);
    const size_t   pidx =  BWT::Encode(*output,*input,n,*indices);
    {
        ios::ocstream fp("bwt.bin"); fp << output;
        std::cerr << "$output      : " << S.of(output)      << std::endl;
        BWT::Decode(*decoded, *output, n, *indices, pidx);
        Y_CHECK( decoded == input );
    }

    {
        EchoModulation echoEnc;
        Y_CHECK(pidx==BWT::Encode(*outputEcho,*input,n,*indices,echoEnc));
        std::cerr << "$outputEcho  : " << S.of(outputEcho)  << std::endl;
        ios::ocstream fp("bwt-echo.bin"); fp << outputEcho;
        auto_ptr<Modulation> pEnc = echoEnc.clone();
    }
    {
        EchoModulation echoDec;
        BWT::Decode(*decoded, *outputEcho, n, *indices, pidx, echoDec);
        Y_CHECK( decoded == input );
    }
    {
        DeltaEncoder deltaEnc;
        Y_CHECK(pidx==BWT::Encode(*outputDelta,*input,n,*indices,deltaEnc));
        ios::ocstream fp("bwt-delta.bin"); fp << outputDelta;
        std::cerr << "$outputDelta : " << S.of(outputDelta) << std::endl;
        auto_ptr<Modulation> pEnc = deltaEnc.clone();
    }
    {
        DeltaDecoder deltaDec;
        BWT::Decode(*decoded, *outputDelta, n, *indices, pidx, deltaDec);
        Y_CHECK( decoded == input );
    }

    {
        MoveToFrontEncoder mtfEnc;
        Y_CHECK(pidx==BWT::Encode(*outputMTF,*input,n,*indices,mtfEnc));
        ios::ocstream fp("bwt-mtf.bin"); fp << outputMTF;
        std::cerr << "$outputMTF   : " << S.of(outputMTF)   << std::endl;
        auto_ptr<Modulation> pEnc = mtfEnc.clone();
    }

    {
        MoveToFrontDecoder mtfDec;
        BWT::Decode(*decoded, *outputMTF, n, *indices, pidx, mtfDec);
        Y_CHECK( decoded == input );
    }




}


Y_UTEST(bwt)
{
    if( argc > 1 )
    {
        vector<char> tmp;
        {
            std::cerr << "Loading" << std::endl;
            {
                const string  fileName = argv[1];
                ios::icstream fp(fileName);
                char          C=0;
                while( fp.query(C) )
                {
                    tmp << C;
                }
            }
            std::cerr << "/done" << std::endl;
        }
        const string input( *tmp, tmp.size() );
        tmp.release();
        do_bwt(input);
    }
}
Y_UTEST_DONE()

