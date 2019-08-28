#ifndef _YOCTO_GRAY_CIPHER_HPP_
#define _YOCTO_GRAY_CIPHER_HPP_ 1

#include "y/crypto/bc/gray.hpp"
#include "y/os/endian.hpp"
#include "y/type/ints.hpp"
#include "y/crypto/bc/ciphers.hpp"
#include "y/code/ilog2.hpp"

namespace upsylon
{

    namespace crypto
    {

        //! cipher built upon gray codes
        template <
        const size_t               BITS,
        const block_cipher::action MODE
        >
        class gray_cipher : public block_cipher {
        public:
            static  const    size_t BYTES = BITS/8; //!< 1,2,4,8
            typedef typename unsigned_int<BYTES>::type unit_type; //!< alias

            //! setup
            explicit gray_cipher( const memory::ro_buffer &k ) throw() :
            block_cipher(""),
            mask_(0)
            {
                const string _ = vformat("GRAY%u-%s",8*unsigned(sizeof(unit_type)), action_text(MODE) );
                cswap(_,name);
                //-- truncated key, padded with 0
                uint8_t *m = (uint8_t *)(void *)&mask_;
                for( size_t i=0; i < BYTES; ++i )
                    m[i] = k.byte_at(i);

            }

            //! cleanup
            virtual ~gray_cipher() throw()
            {
                mask_ = 0;
            }


            //! block size
            virtual size_t size() const throw()
            {
                return sizeof( unit_type );
            }

            //! [en|de]crypt
            virtual void crypt( void *output, const void *input ) throw() {
                crypt( output, input, int2type<MODE>() );
            }

        private:
            unit_type     mask_;

            inline void crypt( void *output, const void *input, int2type<encrypting> ) throw() {
                unit_type wksp = 0;
                memcpy( &wksp, input, sizeof(unit_type) );
                unit_type temp = wksp^mask_;
                temp = swap_be( temp );
                wksp = swap_be( gray<unit_type>::encode( temp ) );
                memcpy( output, &wksp, sizeof(unit_type) );
            }

            inline void crypt( void *output, const void *input, int2type<decrypting> ) throw() {
                unit_type wksp = 0;
                memcpy( &wksp, input, sizeof(unit_type) );
                unit_type temp = swap_be(wksp);
                wksp = swap_be( gray<unit_type>::decode( temp )  ) ^ mask_;
                memcpy( output, &wksp, sizeof(unit_type) );
            }


            Y_DISABLE_COPY_AND_ASSIGN(gray_cipher);
        };


        //! Gray code on 8 bits
        struct gray8 {
            typedef gray_cipher<8,block_cipher::encrypting> encrypter; //!< alias
            typedef gray_cipher<8,block_cipher::decrypting> decrypter; //!< alias

            //! create specialized ciphers
            static inline ciphers *create( const memory::ro_buffer &k )
            {
                const block_cipher::pointer enc = new encrypter(k);
                const block_cipher::pointer dec = new decrypter(k);
                return new ciphers(enc,dec);
            }
        };

        //! Gray code on 16 bits
        struct gray16 {
            typedef gray_cipher<16,block_cipher::encrypting> encrypter; //!< alias
            typedef gray_cipher<16,block_cipher::decrypting> decrypter; //!< alias

            //! create specialized ciphers
            static inline ciphers *create( const memory::ro_buffer &k )
            {
                const block_cipher::pointer enc = new encrypter(k);
                const block_cipher::pointer dec = new decrypter(k);
                return new ciphers(enc,dec);
            }
        };

        //! Gray code on 32 bits
        struct gray32 {
            typedef gray_cipher<32,block_cipher::encrypting> encrypter; //!< alias
            typedef gray_cipher<32,block_cipher::decrypting> decrypter; //!< alias

            //! create specialized ciphers
            static inline ciphers *create( const memory::ro_buffer &k )
            {
                const block_cipher::pointer enc = new encrypter(k);
                const block_cipher::pointer dec = new decrypter(k);
                return new ciphers(enc,dec);
            }
        };

        //! Gray code on 64 bits
        struct gray64 {
            typedef gray_cipher<64,block_cipher::encrypting> encrypter; //!< alias
            typedef gray_cipher<64,block_cipher::decrypting> decrypter; //!< alias

            //! create specialized ciphers
            static inline ciphers *create( const memory::ro_buffer &k )
            {
                const block_cipher::pointer enc = new encrypter(k);
                const block_cipher::pointer dec = new decrypter(k);
                return new ciphers(enc,dec);
            }
        };




    }


}

#endif
