//! \file
#ifndef Y_CRYPTO_BC_OPERATING_INCLUDED
#define Y_CRYPTO_BC_OPERATING_INCLUDED 1

#include "y/crypto/bc/block-cipher.hpp"

namespace upsylon
{
    namespace crypto
    {
        class operating
        {
        public:
            explicit operating( const block_cipher::pointer &e, const block_cipher::pointer &d);
            virtual ~operating() throw();

            //! encrypt block and synchronize last_[plain|crypt]
            void encrypt_block( void *output, const void *input ) throw();

            //! decrypt block and synchronize last_[plain|crypt]
            void decrypt_block( void *output, const void *input ) throw();

        protected:
            block_cipher::pointer encrypter;  //!< encrypter
            block_cipher::pointer decrypter;  //!< decrypter of encrypter
            const size_t          block_size; //!< common block size
            digest                last_plain; //!< last plain block
            digest                last_crypt; //!< last crypt block
            digest                workspace;  //!< a workspace

            void initialize_crypt() throw();  //!< set last crypt to encrypter(last_plain)
            void initialize_plain() throw();  //!< set last plain to decrypter(last_crypt)
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(operating);
        };

    }
    
}

#endif

