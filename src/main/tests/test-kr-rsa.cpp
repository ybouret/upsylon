#include "y/crypto/rsa/key-file.hpp"
#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"
#include "y/string.hpp"

using namespace upsylon;


Y_UTEST(kr_rsa)
{

    if(argc>1)
    {
        const string     filename = argv[1];
        ios::icstream    fp(filename);
        crytpo::key_file kf(fp);



    }
}
Y_UTEST_DONE()

