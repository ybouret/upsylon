//#include "y/crypto/bc/block-cipher-factory.hpp"

#include "y/crypto/bc/aes.hpp"
#include "y/crypto/bc/teac.hpp"
#include "y/crypto/bc/gray-cipher.hpp"

#include "y/crypto/bc/ecb.hpp"
#include "y/crypto/bc/cbc.hpp"

#include "y/utest/run.hpp"
#include "y/os/uuid.hpp"
#include "y/memory/buffers.hpp"
#include "y/hashing/sha1.hpp"

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
    }

    static inline
    void test_ciphers( crypto::ciphers *cph )
    {
        crypto::ciphers::pointer c(cph);
        std::cerr << "[" << c->encrypter->name << "<->" << c->decrypter->name << "]" << std::endl;
        digest P( c->block_size );
        digest C( c->block_size );


        size_t count = 0;
        for(size_t iter=1;iter<4;++iter)
        {
            alea.fill( c->last_plain.rw(), c->block_size );
            c->sync_crypt();

            string ini(c->block_size,as_capacity);
            string enc(c->block_size,as_capacity);
            string dec(c->block_size,as_capacity);

            for(size_t length=0;length<c->block_size-1;++length)
            {
                std::cerr << '.';
                // save state
                P = c->last_plain;
                C = c->last_crypt;

                ini.clear(); enc.clear(); dec.clear();
                for(size_t i=0;i<length;++i)
                {
                    ini += alea.range('A','Z');
                    enc += '\0';
                    dec += '\0';
                }

                c->flush(*enc,*ini,length);

                // restor state
                c->last_plain = P;
                c->last_crypt = C;
                c->flush(*dec,*enc,length);
                Y_ASSERT(dec==ini);
                if(0==(++count%64)) std::cerr << std::endl;
            }
        }
        std::cerr << std::endl;

        digest IV(c->block_size);
        IV.rand();
        std::cerr << "Testing Operating with IV=" << IV << std::endl;

        hashing::sha1 H;

        for(size_t length=0;length<=1000;++length)
        {
            memory::global_buffer_of<char> ini(length);
            memory::global_buffer_of<char> enc(length);
            memory::global_buffer_of<char> dec(length);

            // ECB
            {
                crypto::ecb::encrypter e;
                c->initialize_plain(IV);
                e.write( *c, *enc, *ini, length);
            }

            {
                crypto::ecb::decrypter d;
                c->initialize_plain(IV);
                d.write(*c,*dec,*enc,length);
            }

            {
                const digest Hini = H.md(ini);
                const digest Hend = H.md(dec);
                Y_ASSERT(Hini==Hend);
            }

            

        }
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

    {
        std::cerr << "-- Testing ciphers" << std::endl;
        const uuid key;
        test_ciphers( crypto::aes128::create(key) );
        test_ciphers( crypto::aes192::create(key) );
        test_ciphers( crypto::aes256::create(key) );
        test_ciphers( crypto::teac_of<128>::create(key) );
        test_ciphers( crypto::teac_of<1024>::create(key) );
        test_ciphers( crypto::gray8 ::create(key) );
        test_ciphers( crypto::gray16::create(key) );
        test_ciphers( crypto::gray32::create(key) );
        test_ciphers( crypto::gray64::create(key) );

    }

    
}
Y_UTEST_DONE()

