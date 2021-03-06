#ifndef Y_CRYPTO_BC_CIPHERS_INCLUDED
#define Y_CRYPTO_BC_CIPHERS_INCLUDED 1

#include "y/crypto/bc/block-cipher.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon
{
    namespace crypto
    {

        //! pair of ciphers with internal plain/crypted block
        class ciphers : public counted_object
        {
        public:
            typedef arc_ptr<ciphers> pointer; //!< alias

            virtual ~ciphers() throw(); //!< destructor

            //! setup
            explicit ciphers( const block_cipher::pointer &enc, const block_cipher::pointer &dec);

            block_cipher::pointer encrypter;    //!< an encrypter
            block_cipher::pointer decrypter;    //!< associated decrypter
            const size_t          block_size;   //!< common block size
            digest                last_plain;   //!< last plain block
            digest                last_crypt;   //!< last crypt block
            digest                temporary;    //!< temporary block

            //! flushing
            void flush( void *output, const void *input, const size_t length ) throw();

            //! last_crypt = encrypt( last_plain )
            void sync_crypt() throw();

            //! last_plain = decrypt( last_crypt )
            void sync_plain() throw();

            //! last_plain = data
            void load_plain( const void *data ) throw();

            //! last_crypt = data
            void load_crypt( const void *data ) throw();

            //! temporary = data
            void load_temp_( const void *data) throw();

            //! data = last_plain
            void send_plain( void *data ) const throw();

            //! data = last_crypt
            void send_crypt( void *data ) const throw();

            //! initialize last_plain = 0, last_crypt=encrypt(last_plain)
            void intialize() throw();

            //! initialize with IV, with truncation
            void initialize_plain( const digest &IV ) throw();

            //! initialize IV, with truncation
            void initialize_crypt( const digest &IV ) throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(ciphers);
            void build_flush_mask(const size_t length) throw();

        };

    }
}

#endif

