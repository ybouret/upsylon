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
            static const char name[];
            
            class encrypter : public operating
            {
            public:
                explicit encrypter();
                virtual ~encrypter() throw();

                //! last_plain = IV, last_crypt=E(IV)
                virtual void initialize( ciphers &c, const digest &IV ) throw();

                //! \f$ C_i = E( P_i ) \f$
                virtual void write_block( ciphers &c, void *output, const void *input ) throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(encrypter);
            };

            class decrypter : public operating
            {
            public:
                explicit decrypter();
                virtual ~decrypter() throw();

                //! last_plain = IV, last_crypt=E(IV)
                virtual void initialize( ciphers &c, const digest &IV ) throw();

                //! \f$ P_i = D( C_i ) \f$
                virtual void write_block( ciphers &c, void *output, const void *input ) throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(decrypter);
                
            };

        };

    }
}

#endif

