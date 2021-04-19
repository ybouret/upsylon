#include "y/crypto/rsa/key-file.hpp"
#include "y/crypto/rsa/private-key.hpp"
#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"
#include "y/string.hpp"
#include "y/ptr/auto.hpp"
#include "y/os/real-time-clock.hpp"

using namespace upsylon;

static inline
void check_max_bits(const apn &modulus)
{
    
    const apn    nmax = modulus-1;
    const size_t nbit = nmax.bits()-1;
    std::cerr << "modulus: " << modulus << std::endl;
    std::cerr << "nmax   : " << nmax   << ", bits=" << nmax.bits() << std::endl;
    apn    top = 0;
    for(size_t i=0;i<nbit;++i)
    {
        top <<= 1;
        top |= 1;
    }
    std::cerr << "top    : " << top   << ", bits=" << top.bits() << std::endl;
    
}

#include "y/yap/library.hpp"
#include "y/sequence/vector.hpp"
#include "y/sort/heap.hpp"

static inline apn small_prime(const size_t nbit)
{
    static yap::library &lib = yap::library::instance();
    apn p(alea,nbit);
    return lib.next_prime_(p);
    
}

Y_UTEST(kr_rsa)
{

    if(argc>1)
    {

        auto_ptr<crypto::rsa_public_key>  pub = NULL;
        auto_ptr<crypto::rsa_private_key> prv = NULL;

        {
            const string     filename = argv[1];
            ios::icstream    fp(filename);
            crypto::key_file kf(fp);

            std::cerr << "creating public  key..." << std::endl;
            pub = kf.pub();

            std::cerr << "creating private key..." << std::endl;
            prv = kf.prv();
        }

        prv->check();

        std::cerr << "done" << std::endl;
        const size_t bits = pub->modulus.bits();
        std::cerr << "encryptedBits=" << pub->encryptedBits << std::endl;
        std::cerr << "decryptedBits=" << pub->decryptedBits << std::endl;


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
        
        (std::cerr << std::hex << "encoded: " << encoded << std::endl).flush();
        (std::cerr << std::dec << "        \\_ticks=" << enc << std::endl).flush();
        
        mark                   = real_time_clock::ticks();
        const apn      decoded = apn::mod_exp(encoded,prv->privateExponent, prv->modulus);
        const uint64_t dec     = real_time_clock::ticks()-mark;
        (std::cerr << std::hex << "decoded: " << decoded << std::endl).flush();
        (std::cerr << std::dec << "        \\_ticks=" << dec << std::endl).flush();

        mark                  = real_time_clock::ticks();
        const apn decoded_crt = prv->CRT(encoded);
        const uint64_t    crt = real_time_clock::ticks()-mark;
        (std::cerr << std::hex << "decoded: " << decoded_crt << std::endl).flush();
        (std::cerr << std::dec << "        \\_ticks=" << crt << std::endl).flush();

        Y_CHECK(decoded_crt==decoded);
        Y_CHECK(decoded==message);
    }
    
    {
        std::cerr << std::dec;
        std::cerr << "Testing manual RSA" << std::endl;
        const size_t nbit = 6;
        const apn    p = small_prime(nbit);
        apn q = small_prime(nbit);
        while(q==p) q = small_prime(nbit);
        const apn e = 65537;
        std::cerr << "\tp=" << p << std::endl;
        std::cerr << "\tq=" << q << std::endl;
        std::cerr << "\te=" << e << std::endl;

        const auto_ptr<crypto::rsa_public_key>  pub = crypto::rsa_public_key::pub(p,q,e);
        const auto_ptr<crypto::rsa_private_key> prv = crypto::rsa_private_key::prv(q,p,e); prv->check();
        std::cerr << "publicExponent  : " << pub->publicExponent  << std::endl;
        std::cerr << "privateExponent : " << prv->privateExponent << std::endl;
        std::cerr << "modulus         : " << prv->modulus << std::endl;
        check_max_bits(prv->modulus);
        
        Y_CHECK(pub->modulus==prv->modulus);
        Y_CHECK(pub->publicExponent==prv->publicExponent);
        (std::cerr << "checking..." << std::endl).flush();
        const size_t n = pub->modulus.cast_to<size_t>("modulus");

        vector<apn> crypted(n,as_capacity);
        uint64_t    raw_ticks = 0;
        uint64_t    crt_ticks = 0;
        for(apn P=0;P<pub->modulus;++P)
        {
            (std::cerr << "  " << P << "  \r" ).flush();
            const apn C    = apn::mod_exp(P,pub->publicExponent,pub->modulus);
            uint64_t  mark = real_time_clock::ticks();
            const apn M    = apn::mod_exp(C,prv->privateExponent,prv->modulus);
            raw_ticks     += real_time_clock::ticks() - mark;
            mark           = real_time_clock::ticks();
            const apn D    = prv->CRT(C);
            crt_ticks     += real_time_clock::ticks() - mark;
            Y_ASSERT(M==P);
            Y_ASSERT(D==P);
            crypted.push_back_(C);
        }
        std::cerr << std::endl;
        const double ratio = double(raw_ticks)/double(crt_ticks);
        std::cerr << "crt speed up: " << ratio << std::endl;

        (std::cerr << "sampling..." << std::endl).flush();
        hsort(crypted,apn::compare);
        for(size_t i=1;i<=n;++i)
        {
            const apn P(i-1);
            Y_ASSERT(P==crypted[i]);
        }

        std::cerr << "saving key file..." << std::endl;
        {
            ios::ocstream fp("small.key");
            prv->save_key_file(fp);
        }

        std::cerr << "reloading key file..." << std::endl;
        auto_ptr<crypto::rsa_private_key> sub = NULL;
        {
            const string     filename = "small.key";
            ios::icstream    fp(filename);
            crypto::key_file kf(fp);
            sub = kf.prv();
            sub->check();
        }


    }
}
Y_UTEST_DONE()

Y_UTEST(sc_rsa)
{
    for(int i=1;i<argc;++i)
    {
        auto_ptr<crypto::rsa_public_key>  pub = NULL;
        auto_ptr<crypto::rsa_private_key> prv = NULL;

        {
            const string     filename = argv[1];
            ios::icstream    fp(filename);
            crypto::key_file kf(fp);

            std::cerr << "creating public  key..." << std::endl;
            pub = kf.pub();

            std::cerr << "creating private key..." << std::endl;
            prv = kf.prv();
        }

        std::cerr << "checking..." << std::endl;
        prv->check();
        std::cerr << "done" << std::endl;

        std::cerr << "encryptedBits: " << prv->encryptedBits << std::endl;
        std::cerr << "decryptedBits: " << prv->decryptedBits << std::endl;
        std::cerr.flush();
        const apn Plain(alea,prv->decryptedBits);  (std::cerr << "Plain: " << Plain.to_hex() << std::endl).flush();

        {
            const apn Crypt = pub->pub_encrypt(Plain); (std::cerr << "Crypt: " << Crypt.to_hex() << std::endl).flush();
            const apn Recov = prv->prv_decrypt(Crypt);
            Y_CHECK(Plain==Recov);
        }

        {
            const apn Crypt = prv->prv_encrypt(Plain); (std::cerr << "Crypt: " << Crypt.to_hex() << std::endl).flush();
            const apn Recov = pub->pub_decrypt(Crypt);
            Y_CHECK(Plain==Recov);
        }


    }
}
Y_UTEST_DONE()
