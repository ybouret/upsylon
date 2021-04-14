
//! \file

#ifndef Y_CRYPTO_RSA_PUBLIC_KEY_INCLUDED
#define Y_CRYPTO_RSA_PUBLIC_KEY_INCLUDED 1

#include "y/crypto/rsa/key.hpp"

namespace upsylon
{
    namespace crytpo
    {

#define Y_RSA_KEY_FIELD(NAME) NAME(NAME##_)

        //______________________________________________________________________
        //
        //
        //! public key
        //
        //______________________________________________________________________
        class rsa_public_key : public rsa_key
        {
        public:
            const apn modulus;
            const apn publicExponent;

            explicit rsa_public_key(const apn &modulus_,
                                    const apn &publicExponent_);
            virtual ~rsa_public_key() throw();
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(rsa_public_key);
        };

    }

}

#endif
