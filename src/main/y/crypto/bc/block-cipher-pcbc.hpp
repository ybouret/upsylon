//! \file
#ifndef Y_BLOCK_CIPHER_PCBC_INCLUDED
#define Y_BLOCK_CIPHER_PCBC_INCLUDED 1

#include "y/crypto/bc/operating-block-cipher.hpp"

namespace upsylon {

    namespace crypto {


        //! Propagating Chained Block Cipher
        struct block_cipher_pcbc {

            //! \f$ C_i = E_K( P_i \oplus P_{i-1} \oplus C_{i-1}), P_0 \oplus C_0 = IV \f$
            class encrypter : public operating_block_cipher {
            public:
                explicit encrypter( const pointer &bc, const memory::ro_buffer &iv);
                virtual ~encrypter() throw();

                virtual void        crypt( void *output, const void *input ) throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(encrypter);
            };

            //! \f$ P_i = D_K(C_i) \oplus P_{i-1} \oplus C_{i-1}, P_0 \oplus C_0=IV \f$
            class decrypter : public operating_block_cipher {
            public:
                explicit decrypter( const pointer &bc , const pointer &rc, const memory::ro_buffer &iv );
                virtual ~decrypter() throw();

                virtual void        crypt( void *output, const void *input ) throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(decrypter);

            };


        };


    }

}

#endif
