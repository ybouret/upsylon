#ifndef Y_BLOCK_CIPHER_INCLUDED
#define Y_BLOCK_CIPHER_INCLUDED 1

#include "y/hashing/digest.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon
{

    namespace crypto
    {

        class block_cipher : public counted_object
        {
        public:
            typedef arc_ptr<block_cipher> pointer;
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
