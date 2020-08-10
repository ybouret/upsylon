//#include "y/crypto/bc/block-cipher-factory.hpp"

#include "y/crypto/bc/aes.hpp"
#include "y/crypto/bc/teac.hpp"
#include "y/crypto/bc/gray-cipher.hpp"

#include "y/crypto/bc/ecb.hpp"
#include "y/crypto/bc/cbc.hpp"
#include "y/crypto/bc/ctr.hpp"

#include "y/utest/run.hpp"
#include "y/os/uuid.hpp"
#include "y/memory/zblock.hpp"
#include "y/hashing/sha1.hpp"
#include "y/memory/allocator/global.hpp"

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
        string
        P(n,as_capacity,false),
        C(n,as_capacity,false),
        D(n,as_capacity,false);
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

    template <typename OP>
    static inline void test_op(const digest    &IV,
                               crypto::ciphers &c,
                               const memory::ro_buffer &ini,
                               memory::rw_buffer       &enc,
                               memory::rw_buffer       &dec)
    {
        assert(enc.length()==ini.length());
        assert(dec.length()==ini.length());

        hashing::sha1 H;
        const size_t length = ini.length();
        {
            typename OP::encrypter e;
            e.initialize(c,IV);
            e.write(c, enc.rw(), ini.ro(), length);
        }

        {
            typename OP::decrypter d;
            d.initialize(c,IV);
            d.write(c, dec.rw(), enc.ro(),length);
        }

        {
            const digest Hini = H.md(ini);
            const digest Hend = H.md(dec);
            Y_ASSERT(Hini==Hend);
        }
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

            string ini(c->block_size,as_capacity,false);
            string enc(c->block_size,as_capacity,false);
            string dec(c->block_size,as_capacity,false);

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

        typedef zblock<char,memory::global> zbuf;
        for(size_t length=0;length<=1000;++length)
        {
            zbuf ini(length);
            zbuf enc(length);
            zbuf dec(length);

            test_op<crypto::ecb>(IV,*c,ini,enc,dec);
            test_op<crypto::cbc>(IV,*c,ini,enc,dec);
            test_op<crypto::ctr>(IV,*c,ini,enc,dec);

        }
        std::cerr << std::endl;
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

