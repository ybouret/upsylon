#include "y/mpl/rsa/keys.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/ios/imstream.hpp"

using namespace upsylon;

static const uint8_t rsa_keys_inc[] =
{
#include "rsa-keys.inc"
};


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
    
}
Y_UTEST_DONE()

