
//! \file

#ifndef Y_CRYPTO_RSA_PUBLIC_KEY_INCLUDED
#define Y_CRYPTO_RSA_PUBLIC_KEY_INCLUDED 1

#include "y/crypto/rsa/key.hpp"

namespace upsylon
{
    namespace crypto
    {
        //! helper for constructor
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
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const char CLID[]; //!< "rsa_public_key"

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup
            explicit rsa_public_key(const apn &modulus_,
                                    const apn &publicExponent_);
            //! cleanup
            virtual ~rsa_public_key() throw();

            //__________________________________________________________________
            //
            // interface
            //__________________________________________________________________
            virtual const char *className() const throw();        //!< CLID
            virtual size_t      serialize(ios::ostream &) const;  //!< mod+exp

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! create a manual public key from two primes and an exponent
            static rsa_public_key *pub(const apn &p, const apn &q, const apn &e);


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const apn    modulus;        //!< RSA modulus
            const apn    publicExponent; //!< publicExponent
            


        private:
            Y_DISABLE_COPY_AND_ASSIGN(rsa_public_key);
        };

    }

}

#endif
