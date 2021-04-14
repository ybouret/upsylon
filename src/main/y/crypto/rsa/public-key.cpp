


#include "y/crypto/rsa/public-key.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon
{
    namespace crytpo
    {

        const char rsa_public_key:: CLID[] = "rsa_public_key";

        const char *rsa_public_key:: className() const throw() { return CLID; }

        size_t      rsa_public_key:: serialize(ios::ostream &fp) const
        {
            size_t res = 0;
            res += modulus.serialize(fp);
            res += publicExponent.serialize(fp);
            return res;
        }

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
