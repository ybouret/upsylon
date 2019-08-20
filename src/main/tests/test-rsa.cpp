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
    const mpn p = 11;
    const mpn q = 3;
    const mpn e = 3;

    RSA::SharedKey pub = RSA::PublicKey ::Create(p, q, e);
    RSA::SharedKey prv = RSA::PrivateKey::Create(p, q, e);

    std::cerr << "Modulus: " << pub->modulus << "/" << prv->modulus << std::endl;

    std::cerr << "sizeof(RSA::PublicKey ) = " << sizeof( RSA::PublicKey  ) << std::endl;
    std::cerr << "sizeof(RSA::PrivateKey) = " << sizeof( RSA::PrivateKey ) << std::endl;


}
Y_UTEST_DONE()




