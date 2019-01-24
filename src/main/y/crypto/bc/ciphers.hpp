#ifndef Y_CRYPTO_BC_CIPHERS_INCLUDED
#define Y_CRYPTO_BC_CIPHERS_INCLUDED 1

#include "y/crypto/bc/block-cipher.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon
{
    namespace crypto
    {

        class ciphers : public counted_object
        {
        public:
            typedef arc_ptr<ciphers> pointer;

            virtual ~ciphers() throw();
            explicit ciphers( const block_cipher::pointer &enc, const block_cipher::pointer &dec);

            block_cipher::pointer encrypter;
            block_cipher::pointer decrypter;
            const size_t          block_size;
            digest                last_plain;
            digest                last_crypt;

            //! flushing
            void flush( void *output, const void *input, const size_t length ) throw();

            //! last_crypt = encrypt( last_plain )
            void sync_crypt() throw();

            //! last_plain = decrypt( last_crypt )
            void sync_plain() throw();

            //! last_plain = data
            void save_plain( const void *data ) throw();

            //! last_crypt = data
            void save_crypt( const void *data ) throw();

            //! initialize last_plain = 0, last_crypt=encrypt(last_plain)
            void intialize() throw();

            //! initialize with IV, with truncation
            void initialize( const digest &IV ) throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ciphers);
            void build_flush_mask(const size_t length) throw();

        };

    }
}

#endif

