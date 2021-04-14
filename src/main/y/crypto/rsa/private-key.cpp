

#include "y/crypto/rsa/private-key.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace crytpo
    {


        const char  rsa_private_key:: CLID[] = "rsa_private_key";

        const char *rsa_private_key:: className() const throw() { return CLID; }

        size_t      rsa_private_key:: serialize(ios::ostream &fp) const
        {
            size_t res = 0;
            res += modulus.serialize(fp);
            res += publicExponent.serialize(fp);
            res += privateExponent.serialize(fp);
            res += prime1.serialize(fp);
            res += prime2.serialize(fp);
            res += exponent1.serialize(fp);
            res += exponent2.serialize(fp);
            res += coefficient.serialize(fp);
            return res;
        }


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
                throw exception("%s.modulus/primes error",CLID);
            }

            if(prime1<=prime2)
            {
                throw exception("%s.prime1<=prime2",CLID);
            }

            const apn p1m = prime1-1;
            const apn p2m = prime2-1;

            if( (privateExponent%p1m) != exponent1 )
            {
                throw exception("%s.exponent1 error",CLID);
            }

            if( (privateExponent%p2m) != exponent2 )
            {
                throw exception("%sexponent2 error",CLID);
            }

            if( apn::mod_inv(prime2,prime1) != coefficient )
            {
                throw exception("%s.coefficient error",CLID);
            }

        }

    }

}
