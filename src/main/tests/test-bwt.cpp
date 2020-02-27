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

    information::entropy S;
    std::cerr << "$input       : " << S.of(input)       << std::endl;


    vector<size_t> indices(n,0);
    const size_t   pidx =  bwt::encode(*output,*input,n,*indices);
    {
        ios::ocstream fp("bwt.bin"); fp << output;
        std::cerr << "$output      : " << S.of(output)      << std::endl;
        bwt::decode(*decoded, *output, n, *indices, pidx);
        Y_CHECK( decoded == input );
    }
    {
        EchoModulation echoEnc;
        Y_CHECK(pidx==bwt::encode(*outputEcho,*input,n,*indices,echoEnc));
        std::cerr << "$outputEcho  : " << S.of(outputEcho)  << std::endl;
        ios::ocstream fp("bwt-echo.bin"); fp << outputEcho;
        auto_ptr<Modulation> pEnc = echoEnc.clone();
    }
    {
        EchoModulation echoDec;
        bwt::decode(*decoded, *outputEcho, n, *indices, pidx, echoDec);
        Y_CHECK( decoded == input );
    }
    {
        DeltaEncoder deltaEnc;
        Y_CHECK(pidx==bwt::encode(*outputDelta,*input,n,*indices,deltaEnc));
        ios::ocstream fp("bwt-delta.bin"); fp << outputDelta;
        std::cerr << "$outputDelta : " << S.of(outputDelta) << std::endl;
        auto_ptr<Modulation> pEnc = deltaEnc.clone();
    }
    {
        DeltaDecoder deltaDec;
        bwt::decode(*decoded, *outputDelta, n, *indices, pidx, deltaDec);
        Y_CHECK( decoded == input );
    }

    {
        MoveToFrontEncoder mtfEnc;
        Y_CHECK(pidx==bwt::encode(*outputMTF,*input,n,*indices,mtfEnc));
        ios::ocstream fp("bwt-mtf.bin"); fp << outputMTF;
        std::cerr << "$outputMTF   : " << S.of(outputMTF)   << std::endl;
        auto_ptr<Modulation> pEnc = mtfEnc.clone();
    }

    {
        MoveToFrontDecoder mtfDec;
        bwt::decode(*decoded, *outputMTF, n, *indices, pidx, mtfDec);
        Y_CHECK( decoded == input );
    }




}


Y_UTEST(bwt)
{
    if( argc > 1 )
    {
        string input(32000,as_capacity,false);
        {
            const string  fileName = argv[1];
            ios::icstream fp(fileName);
            char          C=0;
            while( fp.query(C) ) input << C;
        }
        do_bwt(input);
    }
}
Y_UTEST_DONE()

