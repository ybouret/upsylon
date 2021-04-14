#include "y/crypto/rsa/key-file.hpp"
#include "y/crypto/rsa/private-key.hpp"
#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"
#include "y/string.hpp"
#include "y/ptr/auto.hpp"
#include "y/os/real-time-clock.hpp"

using namespace upsylon;


Y_UTEST(kr_rsa)
{


   

    if(argc>1)
    {
        const string     filename = argv[1];
        ios::icstream    fp(filename);
        crypto::key_file kf(fp);

        std::cerr << "creating public  key..." << std::endl;
        auto_ptr<crypto::rsa_public_key> pub = kf.pub();

        std::cerr << "creating private key..." << std::endl;
        auto_ptr<crypto::rsa_private_key> prv = kf.prv();

        prv->check();

        std::cerr << "done" << std::endl;
        const size_t bits = pub->modulus.bits();
        std::cerr << "#bits=" << bits << std::endl;
        {
            ios::ocstream fp("pub.bin");
            pub->serialize_class(fp);
        }

        {
            ios::ocstream fp("prv.bin");
            prv->serialize_class(fp);
        }

        std::cerr << std::hex;
        const apn message(alea,bits/2);
        (std::cerr << "message: " << message << std::endl).flush();
        
        uint64_t       mark    = real_time_clock::ticks();
        const apn      encoded = apn::mod_exp(message,pub->publicExponent,  pub->modulus);
        const uint64_t enc     = real_time_clock::ticks()-mark;
        
        (std::cerr << "encoded: " << encoded << std::endl).flush();
        (std::cerr << "        \\_ticks=" << enc << std::endl).flush();
        
        mark                   = real_time_clock::ticks();
        const apn      decoded = apn::mod_exp(encoded,prv->privateExponent, prv->modulus);
        const uint64_t dec     = real_time_clock::ticks()-mark;
        (std::cerr << "decoded: " << decoded << std::endl).flush();
        (std::cerr << "        \\_ticks=" << dec << std::endl).flush();

        mark                  = real_time_clock::ticks();
        const apn decoded_crt = prv->CRT(encoded);
        const uint64_t    crt = real_time_clock::ticks()-mark;
        (std::cerr << "decoded: " << decoded_crt << std::endl).flush();
        (std::cerr << "        \\_ticks=" << crt << std::endl).flush();

        Y_CHECK(decoded_crt==decoded);
        Y_CHECK(decoded==message);
    }
    
    {
        std::cerr << std::dec;
        std::cerr << "Testing manual RSA" << std::endl;
        const apn p = 61;
        const apn q = 53;
        const apn e = 65537;
        const auto_ptr<crypto::rsa_public_key>  pub = crypto::rsa_public_key::pub(p,q,e);
        const auto_ptr<crypto::rsa_private_key> prv = crypto::rsa_private_key::prv(q,p,e); prv->check();
        std::cerr << "publicExponent  : " << pub->publicExponent  << std::endl;
        std::cerr << "privateExponent : " << prv->privateExponent << std::endl;
        std::cerr << "modulus         : " << prv->modulus << std::endl;
        Y_CHECK(pub->modulus==prv->modulus);
        Y_CHECK(pub->publicExponent==prv->publicExponent);
        (std::cerr << "checking..." << std::endl).flush();
        for(apn P=0;P<pub->modulus;++P)
        {
            (std::cerr << "  " << P << "  \r" ).flush();
            const apn C = apn::mod_exp(P,pub->publicExponent,pub->modulus);
            const apn M = apn::mod_exp(C,prv->privateExponent,prv->modulus);
            const apn D = prv->CRT(C);
            //std::cerr << P << " -> " << C << " -> " << M << "/" << D << std::endl;
            Y_ASSERT(M==P);
            Y_ASSERT(D==P);
        }
        std::cerr << std::endl;
        
        
    }
}
Y_UTEST_DONE()

