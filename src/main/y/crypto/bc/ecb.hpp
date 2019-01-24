//! \file
#ifndef Y_CRYPTO_BC_ECB_INCLUDED
#define Y_CRYPTO_BC_ECB_INCLUDED 1

#include "y/crypto/bc/operating.hpp"

namespace upsylon
{
    namespace crypto
    {
        //! Electronic Code Book

        struct ecb
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
        };

    }
}

#endif

