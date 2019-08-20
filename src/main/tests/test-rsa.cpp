#include "y/mpl/rsa/keys.hpp"
#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"
#include "y/string/tokenizer.hpp"
#include "y/sequence/vector.hpp"
#include "y/codec/base64.hpp"
#include "y/string/convert.hpp"
#include "y/ios/osstream.hpp"
#include "y/ios/imstream.hpp"
#include "y/hashing/sha1.hpp"

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

    std::cerr << "Write Local" << std::endl;
    {
        string data;
        {
            ios::osstream fp(data);
            size_t nw = 0;
            nw += pub->serialize(fp);
            nw += prv->serialize(fp);
            Y_ASSERT(data.size()==nw);
            std::cerr << "Wrote " << nw << " bytes" << std::endl;
        }

        hashing::sha1 H;
        {
            ios::imstream  fp(data);
            RSA::SharedKey new_pub = RSA::Key::Read(fp);
            RSA::SharedKey new_prv = RSA::Key::Read(fp);
            Y_CHECK( RSA::Key::Public  == new_pub->type );
            Y_CHECK( RSA::Key::Private == new_prv->type );
            {
                const digest pub1 = pub->md(H);
                const digest pub2 = new_pub->md(H);
                Y_CHECK(pub1==pub2);
            }
            {
                const digest prv1 = prv->md(H);
                const digest prv2 = new_prv->md(H);
                Y_CHECK(prv1==prv2);
            }

        }
    }


    std::cerr << "sizeof(RSA::PublicKey ) = " << sizeof( RSA::PublicKey  ) << std::endl;
    std::cerr << "sizeof(RSA::PrivateKey) = " << sizeof( RSA::PrivateKey ) << std::endl;

    const size_t mbits = pub->maxbits;
    std::cerr << "mbits=" << mbits << std::endl;

    for(size_t i=1;i<=8;++i)
    {
        const mpn M(mbits,alea);
        const mpn C = pub->pub(M);
        { const mpn C2 = prv->pub(M); Y_ASSERT(C2==C); }
        const mpn D = prv->prv(C);
        std::cerr << M << "->" << C << "->" << D << std::endl;
        Y_ASSERT(M==D);
    }


}
Y_UTEST_DONE()

Y_UTEST(rsa_keys)
{
    if(argc>1)
    {
        vector<RSA::SharedKey> keys(128,as_capacity);
        {
            const string  datafile = argv[1];
            ios::icstream fp(datafile);
            char C = 0;
            while( fp.query(C) )
            {
                fp.store(C);
                const RSA::SharedKey key = RSA::Key::Read(fp);
                keys.push_back(key);
            }
        }
        const size_t nk = keys.size();
        std::cerr << "Loaded " << nk << " keys" << std::endl;
        for(size_t i=1;i<=nk;++i)
        {
            std::cerr << "..bits=" << keys[i]->modulus.bits() << std::endl;
        }
    }
}
Y_UTEST_DONE()



