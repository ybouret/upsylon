
//! \file
#ifndef Y_CRYPTO_BC_CTR_INCLUDED
#define Y_CRYPTO_BC_CTR_INCLUDED 1

#include "y/crypto/bc/operating.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon
{
    namespace crypto
    {
        //! CounTeR Block Cipher
        struct ctr
        {
            static const char name[];

            class context
            {
            public:
                virtual ~context() throw();

                void start( ciphers &c, const digest &IV );

            protected:
                explicit context() throw();
                
                auto_ptr<digest> counter;
                auto_ptr<digest> crypted;

                void make( const size_t block_size );
                void next( ciphers &c ) throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(context);
                void kill() throw();
            };

            class encrypter : public context, public operating
            {
            public:
                explicit encrypter();
                virtual ~encrypter() throw();

                virtual void initialize( ciphers &c, const digest &IV );
                virtual void write_block( ciphers &c, void *output, const void *input ) throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(encrypter);
            };

            class decrypter : public context, public operating
            {
            public:
                explicit decrypter();
                virtual ~decrypter() throw();

                virtual void initialize( ciphers &c, const digest &IV );
                virtual void write_block( ciphers &c, void *output, const void *input ) throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(decrypter);
            };
        };

    }
}

#endif

