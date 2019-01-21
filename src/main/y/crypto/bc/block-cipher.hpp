#ifndef Y_BLOCK_CIPHER_INCLUDED
#define Y_BLOCK_CIPHER_INCLUDED 1

#include "y/hashing/digest.hpp"

namespace upsylon {

    namespace crypto {

        class block_cipher : public virtual object
        {
        public:
            enum action { encrypting, decrypting };

            virtual ~block_cipher() throw();

            virtual size_t        size() const throw() = 0; //!< block size
            virtual void          crypt( void *output, const void *input ) throw() = 0; //!< blocks of this->size() !

        protected:
            explicit block_cipher() throw();


        private:
            Y_DISABLE_COPY_AND_ASSIGN(block_cipher);
        };

    }

}

#endif
