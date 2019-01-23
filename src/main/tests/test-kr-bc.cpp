//#include "y/crypto/bc/block-cipher-factory.hpp"

#include "y/crypto/bc/aes.hpp"
#include "y/crypto/bc/teac.hpp"
#include "y/crypto/bc/gray-cipher.hpp"

#include "y/crypto/bc/operating.hpp"

#include "y/utest/run.hpp"
#include "y/os/uuid.hpp"

using namespace upsylon;

namespace
{
    template <typename ENCRYPTER, typename DECRYPTER>
    static inline void test_cipher()
    {
        const uuid                    key;
        crypto::block_cipher::pointer enc = new ENCRYPTER(key);
        crypto::block_cipher::pointer dec = new DECRYPTER(key);
        Y_ASSERT(enc->size()==dec->size());
        std::cerr << "block_size=" << enc->size() << std::endl;
        std::cerr << "   Encrypt=" << enc->name   << std::endl;
        std::cerr << "   Decrypt=" << dec->name   << std::endl;

        const size_t n = enc->size();
        string       P(n,as_capacity), C(n,as_capacity), D(n,as_capacity);
        for(size_t i=0;i<n;++i)
        {
            P += alea.range('a','z');
            C += '\0';
            D += '\0';
        }
        enc->crypt(*C,*P);
        dec->crypt(*D,*C);
        std::cerr << "P=" << P << std::endl;
        std::cerr << "C="; C.display_printable(std::cerr) << std::endl;
        std::cerr << "D=" << D << std::endl;
        Y_CHECK(P==D);

        std::cerr << "Creating OP" << std::endl;
        crypto::operating op(enc,dec);
        std::cerr << std::endl;
        

    }
}

Y_UTEST(kr_bc)
{

    test_cipher<crypto::aes128::encrypter,crypto::aes128::decrypter>();
    test_cipher<crypto::aes192::encrypter,crypto::aes192::decrypter>();
    test_cipher<crypto::aes256::encrypter,crypto::aes256::decrypter>();
    test_cipher<crypto::teac<128,crypto::block_cipher::encrypting>,crypto::teac<128,crypto::block_cipher::decrypting> >();
    test_cipher<crypto::teac<1024,crypto::block_cipher::encrypting>,crypto::teac<1024,crypto::block_cipher::decrypting> >();
    test_cipher<crypto::gray8::encrypter, crypto::gray8:: decrypter>();
    test_cipher<crypto::gray16::encrypter,crypto::gray16::decrypter>();
    test_cipher<crypto::gray32::encrypter,crypto::gray32::decrypter>();
    test_cipher<crypto::gray64::encrypter,crypto::gray64::decrypter>();

    
}
Y_UTEST_DONE()

