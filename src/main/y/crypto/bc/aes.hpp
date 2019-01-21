//! \file
#ifndef Y_CRYPTO_BC_AES_INCLUDED
#define Y_CRYPTO_BC_AES_INCLUDED 1

#include "y/crypto/bc/block-cipher.hpp"


namespace upsylon {

    namespace crypto {

        class aes : public block_cipher {
        public:

            struct context {
                int       nr;           /*!<  number of rounds  */
                uint32_t *rk;           /*!<  AES round keys    */
                uint32_t  buf[68];      /*!<  unaligned data    */
            };

            virtual ~aes() throw();

            virtual size_t      size() const throw(); //!< 16=128bits
            virtual void        crypt( void *output, const void *input ) throw();


        protected:
            explicit aes(const char *, const block_cipher::action mode) throw();
            const char *name_;
            aes::context ctx_;

        private:
            static  void   encrypt( aes::context *ctx, void *target, const void *source ) throw();
            static  void   decrypt( aes::context *ctx, void *target, const void *source ) throw();
            Y_DISABLE_COPY_AND_ASSIGN(aes);
            void       (*run_)(aes::context *ctx, void *target, const void *source);

        };



        namespace aes128 {

            class encrypter : public aes
            {
            public:
                explicit encrypter( const memory::ro_buffer &key ) throw();
                virtual ~encrypter() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(encrypter);
            };

            class decrypter : public aes
            {
            public:
                explicit decrypter( const memory::ro_buffer &key ) throw();
                virtual ~decrypter() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(decrypter);
            };

        }


        namespace aes192 {

            class encrypter : public aes
            {
            public:
                explicit encrypter( const memory::ro_buffer &key ) throw();
                virtual ~encrypter() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(encrypter);
            };

            class decrypter : public aes
            {
            public:
                explicit decrypter( const memory::ro_buffer &key ) throw();
                virtual ~decrypter() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(decrypter);
            };

        }

        namespace aes256 {

            class encrypter : public aes
            {
            public:
                explicit encrypter( const memory::ro_buffer &key ) throw();
                virtual ~encrypter() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(encrypter);
            };

            class decrypter : public aes
            {
            public:
                explicit decrypter( const memory::ro_buffer &key ) throw();
                virtual ~decrypter() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(decrypter);
            };

        }

    }

}

#endif
