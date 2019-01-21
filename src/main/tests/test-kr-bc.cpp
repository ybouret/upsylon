#include "y/crypto/bc/block-cipher-factory.hpp"

#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(kr_bc)
{
    crypto::block_cipher_factory &bcf = crypto::block_cipher_factory::instance();
    for( crypto::block_cipher_factory::iterator i=bcf.begin();i!=bcf.end();++i)
    {

        std::cerr << "id=" << i.key() << std::endl;
    }
}
Y_UTEST_DONE()

