#include "y/mpl/rsa/keys.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/ios/imstream.hpp"

using namespace upsylon;

static const uint8_t rsa_keys_inc[] =
{
#include "rsa-keys.inc"
};

#include "y/string/convert.hpp"

Y_UTEST(rsa_io)
{
    size_t shift = 0;
    vector<RSA::SharedKey> keys;
    {
        ios::imstream fp( rsa_keys_inc, sizeof(rsa_keys_inc) );
        while( fp.is_active() )
        {
            const RSA::SharedKey key = RSA::Key::Read(fp,shift);
            keys.push_back(key);
        }
    }
    std::cerr << "Loaded " << keys.size() << " keys" << std::endl;
    if(keys.size()<=0)
    {
        return 0;
    }
    for(size_t i=1;i<=keys.size();++i)
    {
        std::cerr << (*keys[i]).modulus.bits() << std::endl;
    }

    for(int i=1;i<argc;++i)
    {
        const size_t    j   = 1 + (string_convert::to<size_t>( argv[i], "index" )%keys.size());
        const RSA::Key &key = *keys[j];
        std::cerr << "Testing Key with #bits=" << key.modulus.bits() << std::endl;

        {
            const mpn M( key.maxbits, alea );
            std::cerr << "\tencoding with public key..." << std::endl;
            const mpn C = key.pub(M);
            std::cerr << "\tdecoding with private key..." << std::endl;
            const mpn D = key.prv(C);
            Y_ASSERT(D==M);
        }

        {
            const mpn M( key.maxbits, alea );
            std::cerr << "\tencoding with private key..." << std::endl;
            const mpn C = key.prv(M);
            std::cerr << "\tdecoding with public key..." << std::endl;
            const mpn D = key.pub(C);
            Y_ASSERT(D==M);
        }
    }



}
Y_UTEST_DONE()

