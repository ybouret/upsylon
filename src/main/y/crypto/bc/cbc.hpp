//! \file
#ifndef Y_CRYPTO_BC_CBC_INCLUDED
#define Y_CRYPTO_BC_CBC_INCLUDED 1

#include "y/crypto/bc/operating.hpp"

namespace upsylon
{
    namespace crypto
    {
        //! Cipher Block Chaining
        struct cbc
        {
            static const char name[]; //!< "CBC"

            //! encrypter
            class encrypter : public operating
            {
            public:
                explicit encrypter();         //!< setup
                virtual ~encrypter() throw(); //!< destructor

                //! \f$ C_0=IV \f$
                virtual void initialize( ciphers &c, const digest &IV ) throw();

                //! \f$ C_i = E(P_i \oplus C_{i-1})\f$
                virtual void write_block( ciphers &c, void *output, const void *input ) throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(encrypter);
            };

            //! decrypter
            class decrypter : public operating
            {
            public:
                explicit decrypter();         //!< setup
                virtual ~decrypter() throw(); //!< desctructor

                //! \f$ C_0=IV \f$
                virtual void initialize( ciphers &c, const digest &IV ) throw();

                //! \f$ P_i = D(C_i) \oplus C_{i-1}\f$
                virtual void write_block( ciphers &c, void *output, const void *input ) throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(decrypter);
            };
        };

    }
}

#endif

