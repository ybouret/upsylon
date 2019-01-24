//! \file
#ifndef Y_CRYPTO_BC_CBC_INCLUDED
#define Y_CRYPTO_BC_CBC_INCLUDED 1

#include "y/crypto/bc/operating.hpp"

namespace upsylon
{
    namespace crypto
    {
        //! Cipher Block Chaining
        struct cbc
        {

            class encrypter : public operating
            {
            public:
                explicit encrypter();
                virtual ~encrypter() throw();

                
                virtual void write_block( ciphers &c, void *output, const void *input ) throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(encrypter);
            };

            class decrypter : public operating
            {
            public:
                explicit decrypter();
                virtual ~decrypter() throw();

                virtual void write_block( ciphers &c, void *output, const void *input ) throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(decrypter);
            };
        };

    }
}

#endif

