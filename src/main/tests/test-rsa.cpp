#include "y/mpl/rsa/keys.hpp"
#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"
#include "y/string/tokenizer.hpp"
#include "y/sequence/vector.hpp"
#include "y/codec/base64.hpp"
#include "y/string/convert.hpp"

using namespace upsylon;


Y_UTEST(rsa)
{
#if 0
    const mpn p = 11;
    const mpn q = 3;
    const mpn e = 3;
#endif

    if( argc < 3 ) return 0;
    const mpn p = argv[1];
    const mpn q = argv[2];
    const mpn e = argv[3];

    RSA::SharedKey pub = RSA::PublicKey ::Create(p, q, e);
    RSA::SharedKey prv = RSA::PrivateKey::Create(p, q, e);

    std::cerr << "PublicKey:" << std::endl;
    pub->print(std::cerr);

    std::cerr << "PrivateKey:" << std::endl;
    prv->print(std::cerr);


    std::cerr << "sizeof(RSA::PublicKey ) = " << sizeof( RSA::PublicKey  ) << std::endl;
    std::cerr << "sizeof(RSA::PrivateKey) = " << sizeof( RSA::PrivateKey ) << std::endl;


}
Y_UTEST_DONE()




