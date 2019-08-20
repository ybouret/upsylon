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
    vector<RSA::SharedKey> keys;
    {
        ios::imstream fp( rsa_keys_inc, sizeof(rsa_keys_inc) );
        char          C = 0;
        while( fp.query(C) )
        {
            fp.store(C);
            const RSA::SharedKey key = RSA::Key::Read(fp);
            keys.push_back(key);
        }
    }
    std::cerr << "Loaded " << keys.size() << " keys" << std::endl;
    if(keys.size()<=0)
    {
        return 0;
    }

    for(int i=1;i<argc;++i)
    {
        const size_t    j   = 1 + (string_convert::to<size_t>( argv[i], "index" )%keys.size());
        const RSA::Key &key = *keys[j];
        std::cerr << "Testing Key with #bits=" << key.modulus.bits() << std::endl;
        {
            const mpn M( key.maxbits, alea );
            const mpn C = key.pub(M);
            const mpn D = key.prv(C);
            Y_ASSERT(D==M);
        }

        {
            const mpn M( key.maxbits, alea );
            const mpn C = key.prv(M);
            const mpn D = key.pub(C);
            Y_ASSERT(D==M);
        }
    }



}
Y_UTEST_DONE()

