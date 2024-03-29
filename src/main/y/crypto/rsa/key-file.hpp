//! \file

#ifndef Y_CRYPTO_RSA_KEY_FILE_INCLUDED
#define Y_CRYPTO_RSA_KEY_FILE_INCLUDED 1

#include "y/yap/natural.hpp"
#include "y/associative/hash/map.hpp"
namespace upsylon
{
    namespace crypto
    {
        //______________________________________________________________________
        //
        //! a database of apn
        //______________________________________________________________________
        typedef hash_map<string,apn> named_keys;

        class rsa_private_key; //!< forward declaration
        class rsa_public_key;  //!< forward declaration

        //______________________________________________________________________
        //
        //
        //! parsing a key file produced by openssl
        //
        //______________________________________________________________________
        class key_file : public named_keys
        {
        public:
            explicit key_file(ios::istream &); //!< parse
            virtual ~key_file() throw();       //!< cleanup

            const apn & operator[](const char *id) const; //!< access

            rsa_public_key  *pub() const; //!< return a public key
            rsa_private_key *prv() const; //!< return a private key

        private:
            Y_DISABLE_COPY_AND_ASSIGN(key_file);
        };

    }
}

#endif

