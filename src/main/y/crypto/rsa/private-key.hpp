

//! \file

#ifndef Y_CRYPTO_RSA_PRIVATE_KEY_INCLUDED
#define Y_CRYPTO_RSA_PRIVATE_KEY_INCLUDED 1

#include "y/crypto/rsa/public-key.hpp"

namespace upsylon
{
    namespace crytpo
    {


        //______________________________________________________________________
        //
        //
        //! private key
        //
        //______________________________________________________________________
        class rsa_private_key : public rsa_public_key
        {
        public:
            const apn privateExponent; //!< private exponent
            const apn prime1;          //!< first prime
            const apn prime2;          //!< second prime, prime2<prime1
            const apn exponent1;       //!< precomputed exponent
            const apn exponent2;       //!< precomputed exponent
            const apn coefficient;     //!< precomputed exponent


            //! setup
            explicit rsa_private_key(const apn &modulus_,
                                     const apn &publicExponent_,
                                     const apn &privateExponent_,
                                     const apn &prime1_,
                                     const apn &prime2_,
                                     const apn &exponent1_,
                                     const apn &exponent2_,
                                     const apn &coefficient_);

            //! cleanup
            virtual ~rsa_private_key() throw();

            //! check consistency
            void check() const;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(rsa_private_key);
        };

    }

}

#endif
