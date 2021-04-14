

#include "y/crypto/rsa/private-key.hpp"
#include "y/exception.hpp"
#include "y/type/aliasing.hpp"


namespace upsylon
{
    namespace crypto
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
            aliasing::_(privateExponent).ldz();
            aliasing::_(prime1).ldz();
            aliasing::_(prime2).ldz();
            aliasing::_(exponent1).ldz();
            aliasing::_(exponent2).ldz();
            aliasing::_(coefficient).ldz();
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


        apn rsa_private_key:: CRT(const apn &C) const
        {
            apn       M1 = apn::mod_exp(C,exponent1,prime1);
            const apn M2 = apn::mod_exp(C,exponent2,prime2);
            while(M1<M2) M1 += prime1;
#if 0
            M1-=M2;
            M1 *= coefficient;
            M1 %= prime1;
            M1 *= prime2;
            return M1+M2;
#endif

            return ( ( ( (M1-=M2) *= coefficient ) %= prime1 ) *= prime2 ) + M2;

        }


        rsa_private_key * rsa_private_key:: prv(const apn &p, const apn &q, const apn &e)
        {
            if(p==q) throw exception("%s.pub(p=q)",CLID);
            apn prime1 = p;
            apn prime2 = q;
            if(prime2>prime1) prime1.xch(prime2);
            assert(prime2<prime1);

            const apn modulus         = prime1*prime2;
            const apn publicExponent  = e;
            const apn p1m             = prime1-1;
            const apn p2m             = prime2-1;
            const apn lam             = apn::lcm(p1m,p2m);
            const apn privateExponent = apn::mod_inv(publicExponent,lam);
            const apn exponent1       = privateExponent%p1m;
            const apn exponent2       = privateExponent%p2m;
            const apn coefficient     = apn::mod_inv(prime2,prime1);

            return new rsa_private_key(modulus,publicExponent,privateExponent,prime1,prime2,exponent1,exponent2,coefficient);
        }

    }

}
