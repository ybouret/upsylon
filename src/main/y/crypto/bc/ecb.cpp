
#include "y/crypto/bc/ecb.hpp"



namespace upsylon
{
    namespace crypto
    {
        ecb::encrypter:: encrypter() : operating()
        {
        }

        ecb::encrypter:: ~encrypter() throw()
        {
        }

        void ecb:: encrypter:: write_block( ciphers &c, void *output, const void *input ) throw()
        {
            c.save_plain(input);
            c.encrypter->crypt(output,input);
            c.save_crypt(output);
        }


    }
}
