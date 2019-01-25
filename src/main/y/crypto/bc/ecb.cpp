
#include "y/crypto/bc/ecb.hpp"



namespace upsylon
{
    namespace crypto
    {
        const char ecb::name[] = "ECB";

        ecb::encrypter:: encrypter() : operating()
        {
        }

        ecb::encrypter:: ~encrypter() throw()
        {
        }

        void ecb:: encrypter:: write_block( ciphers &c, void *output, const void *input ) throw()
        {
            c.load_plain(input);
            c.encrypter->crypt(output,input);
            c.load_crypt(output);
        }

        void ecb::encrypter:: initialize( ciphers &c, const digest &IV ) throw()
        {
            c.initialize_plain(IV);
        }

    }
}


namespace upsylon
{
    namespace crypto
    {
        ecb::decrypter:: decrypter() : operating()
        {
        }

        ecb::decrypter:: ~decrypter() throw()
        {
        }

        void ecb:: decrypter:: write_block( ciphers &c, void *output, const void *input ) throw()
        {
            c.load_crypt(input);
            c.decrypter->crypt(output,input);
            c.load_plain(output);
        }

        void ecb:: decrypter:: initialize( ciphers &c, const digest &IV ) throw()
        {
            c.initialize_plain(IV);
        }

    }
}
