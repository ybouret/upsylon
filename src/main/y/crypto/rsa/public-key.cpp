


#include "y/crypto/rsa/public-key.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon
{
    namespace crytpo
    {

        rsa_public_key:: ~rsa_public_key() throw()
        {
            aliasing::_(modulus).ldz();
            aliasing::_(publicExponent).ldz();

        }

        rsa_public_key:: rsa_public_key(const apn &modulus_,
                                        const apn &publicExponent_) :
        Y_RSA_KEY_FIELD(modulus),
        Y_RSA_KEY_FIELD(publicExponent)
        {

        }

    }

}
