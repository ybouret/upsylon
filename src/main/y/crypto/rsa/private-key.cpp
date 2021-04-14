

#include "y/crypto/rsa/private-key.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace crytpo
    {
        static const char fn[] = "rsa_private_key: ";

        rsa_private_key:: ~rsa_private_key() throw()
        {
        }

        rsa_private_key:: rsa_private_key(const apn &modulus_,
                                          const apn &publicExponent_,
                                          const apn &privateExponent_,
                                          const apn &prime1_,
                                          const apn &prime2_,
                                          const apn &exponent1_,
                                          const apn &exponent2_,
                                          const apn &coefficient_) :
        rsa_public_key(modulus_,publicExponent_),
        Y_RSA_KEY_FIELD(privateExponent),
        Y_RSA_KEY_FIELD(prime1),
        Y_RSA_KEY_FIELD(prime2),
        Y_RSA_KEY_FIELD(exponent1),
        Y_RSA_KEY_FIELD(exponent2),
        Y_RSA_KEY_FIELD(coefficient)
        {

        }

        void rsa_private_key:: check() const
        {
            if(modulus!=prime1*prime2)
            {
                throw exception("%smodulus/primes error",fn);
            }

            if(prime1<=prime2)
            {
                throw exception("%sprime1<=prime2",fn);
            }

            const apn p1m = prime1-1;
            const apn p2m = prime2-1;

            if( (privateExponent%p1m) != exponent1 )
            {
                throw exception("%sexponent1",fn);
            }

            if( (privateExponent%p2m) != exponent2 )
            {
                throw exception("%sexponent2",fn);
            }

        }

    }

}
