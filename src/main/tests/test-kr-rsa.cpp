#include "y/crypto/rsa/key-file.hpp"
#include "y/crypto/rsa/private-key.hpp"
#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"
#include "y/string.hpp"
#include "y/ptr/auto.hpp"

using namespace upsylon;


Y_UTEST(kr_rsa)
{

    if(argc>1)
    {
        const string     filename = argv[1];
        ios::icstream    fp(filename);
        crytpo::key_file kf(fp);

        std::cerr << "creating public  key..." << std::endl;
        auto_ptr<crytpo::rsa_public_key> pub = kf.pub();

        std::cerr << "creating private key..." << std::endl;
        auto_ptr<crytpo::rsa_private_key> prv = kf.prv();

        prv->check();

        std::cerr << "done" << std::endl;

        {
            ios::ocstream fp("pub.bin");
            pub->serialize_class(fp);
        }

        {
            ios::ocstream fp("prv.bin");
            prv->serialize_class(fp);
        }

        std::cerr << std::hex;
        const apn message = 0x1234;
        const apn encoded = apn::mod_exp(message,pub->publicExponent,  pub->modulus);
        const apn decoded = apn::mod_exp(encoded,prv->privateExponent, prv->modulus);
        std::cerr << "message: " << message << std::endl;
        std::cerr << "encoded: " << encoded << std::endl;
        std::cerr << "decoded: " << decoded << std::endl;


    }
}
Y_UTEST_DONE()

