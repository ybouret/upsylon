//! \file
#ifndef Y_BLOCK_CIPHER_CFB_INCLUDED
#define Y_BLOCK_CIPHER_CFB_INCLUDED 1

#include "y/crypto/bc/operating-block-cipher.hpp"

namespace upsylon {

    namespace crypto {


        //! Cipher FeedBack
        struct block_cipher_cfb {

            //! \f$ C_i = E_K(C_{i-1}) \oplus P_i, C_0 = IV \f$
            class encrypter : public operating_block_cipher
            {
            public:
                explicit encrypter( const pointer &bc, const memory::ro_buffer &iv);
                virtual ~encrypter() throw();

                virtual void        crypt( void *output, const void *input ) throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(encrypter);
            };

            //! \f$ P_i = E_K(C_{i-1}) \oplus C_{i}, C_0=IV \f$
            class decrypter : public operating_block_cipher
            {
            public:
                explicit decrypter( const pointer &bc, const memory::ro_buffer &iv );
                virtual ~decrypter() throw();

                virtual void        crypt( void *output, const void *input ) throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(decrypter);
            };


        };


    }

}

#endif
