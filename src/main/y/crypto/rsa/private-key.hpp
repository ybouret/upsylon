

//! \file

#ifndef Y_CRYPTO_RSA_PRIVATE_KEY_INCLUDED
#define Y_CRYPTO_RSA_PRIVATE_KEY_INCLUDED 1

#include "y/crypto/rsa/public-key.hpp"

namespace upsylon
{
    namespace crypto
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
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const char CLID[]; //!< "rsa_private_key";

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

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

            //__________________________________________________________________
            //
            // interface
            //__________________________________________________________________
            virtual const char *className() const throw();       //!< CLID
            virtual size_t      serialize(ios::ostream &) const; //!< pub+all fields

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! check consistency
            void check() const;

            //! apply by CRT
            apn CRT(const apn &M) const;

            //! create a test key
            static rsa_private_key *prv(const apn &p, const apn &q, const apn &e);


            //! encrypt a plain message, P.bits() <= decryptedBits
            apn prv_encrypt( const apn &P ) const;

            //! decrypt a coded message, C < modulus check plain validity
            apn prv_decrypt( const apn &C ) const;

            //! save as key-file
            void save_key_file(ios::ostream &) const;

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const apn privateExponent; //!< privateExponent
            const apn prime1;          //!< first prime
            const apn prime2;          //!< second prime, prime2<prime1
            const apn exponent1;       //!< privateExponent % (prime1-1)
            const apn exponent2;       //!< privateExponent % (prime2-1)
            const apn coefficient;     //!< mod_inv(prime2,prime1)

        private:
            Y_DISABLE_COPY_AND_ASSIGN(rsa_private_key);
        };

    }

}

#endif
