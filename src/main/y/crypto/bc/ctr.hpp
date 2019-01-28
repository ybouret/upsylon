
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
            static const char name[]; //!< "CTR"

            //! context to hold counter
            class context
            {
            public:
                virtual ~context() throw(); //!< desctructor

                //! check internal memory and set counter to IV, truncated
                void start( ciphers &c, const digest &IV );

            protected:

                auto_ptr<digest> counter; //!< counter
                auto_ptr<digest> crypted; //!< encrypted counter

                explicit context() throw();           //!< setup
                void make( const size_t block_size ); //!< check sizes
                void next( ciphers &c ) throw();      //!< encrypt increased counter

            private:
                Y_DISABLE_COPY_AND_ASSIGN(context);
                void kill() throw();
            };

            //! encrypter
            class encrypter : public context, public operating
            {
            public:
                explicit encrypter();         //!< setup
                virtual ~encrypter() throw(); //!< destructor

                //! initialize internal context
                virtual void initialize( ciphers &c, const digest &IV );

                //! operating API
                virtual void write_block( ciphers &c, void *output, const void *input ) throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(encrypter);
            };

            //! decrypter
            class decrypter : public context, public operating
            {
            public:
                explicit decrypter();         //!< setup
                virtual ~decrypter() throw(); //!< destructor

                //! initialize internal context
                virtual void initialize( ciphers &c, const digest &IV );

                //! operating API
                virtual void write_block( ciphers &c, void *output, const void *input ) throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(decrypter);
            };
        };

    }
}

#endif

