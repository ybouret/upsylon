//! \file
#ifndef Y_CRYPTO_BC_AES_INCLUDED
#define Y_CRYPTO_BC_AES_INCLUDED 1

#include "y/crypto/bc/block-cipher.hpp"


namespace upsylon {

    namespace crypto {

        //! AES implementations
        class aes : public block_cipher {
        public:

            //! context from RFC
            struct context {
                int       nr;           /*!<  number of rounds  */
                uint32_t *rk;           /*!<  AES round keys    */
                uint32_t  buf[68];      /*!<  unaligned data    */
            };

            //! cleanup
            virtual ~aes() throw();

            virtual size_t      size() const throw(); //!< 16=128bits
            virtual void        crypt( void *output, const void *input ) throw(); //!< 128bits


        protected:
            //! setup
            explicit aes(const char *depth, const block_cipher::action mode);
            aes::context ctx_; //!< internal context

        private:
            static  void   encrypt( aes::context *ctx, void *target, const void *source ) throw();
            static  void   decrypt( aes::context *ctx, void *target, const void *source ) throw();
            Y_DISABLE_COPY_AND_ASSIGN(aes);
            void       (*run_)(aes::context *ctx, void *target, const void *source);

        };



        namespace aes128 {

            //! AES-128 encrypt
            class encrypter : public aes
            {
            public:
                //! setup
                explicit encrypter( const memory::ro_buffer &k );

                //! desctructor
                virtual ~encrypter() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(encrypter);
            };

            //! AES-128 decrypt
            class decrypter : public aes
            {
            public:
                //! setup
                explicit decrypter( const memory::ro_buffer &k );

                //! desctructor
                virtual ~decrypter() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(decrypter);
            };

        }


        namespace aes192 {

            //! AES-192 encrypt
            class encrypter : public aes
            {
            public:
                //! setup
                explicit encrypter( const memory::ro_buffer &k );

                //! destructor
                virtual ~encrypter() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(encrypter);
            };

            //! AES-192 decrypt
            class decrypter : public aes
            {
            public:
                //! setup
                explicit decrypter( const memory::ro_buffer &k );

                //! destructor
                virtual ~decrypter() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(decrypter);
            };

        }

        namespace aes256 {

            //! AES-256 encrypt
            class encrypter : public aes
            {
            public:
                //! setup
                explicit encrypter( const memory::ro_buffer &k );

                //! destructor
                virtual ~encrypter() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(encrypter);
            };

            //! AES-256 decrypt
            class decrypter : public aes
            {
            public:
                //! setup
                explicit decrypter( const memory::ro_buffer &k );

                //! destructor
                virtual ~decrypter() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(decrypter);
            };

        }

    }

}

#endif
