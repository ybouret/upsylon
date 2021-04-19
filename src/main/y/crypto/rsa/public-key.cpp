


#include "y/crypto/rsa/public-key.hpp"
#include "y/type/aliasing.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace crypto
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
        Y_RSA_KEY_FIELD(publicExponent),
        extremum(modulus-1),
        encryptedBits(extremum.bits()),
        decryptedBits(encryptedBits-1)
        {
            
        }

        rsa_public_key * rsa_public_key:: pub(const apn &p, const apn &q, const apn &e)
        {
            if(p==q) throw exception("%s.pub(p=q)",CLID);
            const apn n = p*q;
            return new rsa_public_key(n,e);
        }

        apn rsa_public_key:: pub_encrypt( const apn &P ) const
        {
            assert(P.bits()<=decryptedBits);
            return apn::mod_exp(P,publicExponent,modulus);
        }

        apn rsa_public_key:: pub_decrypt( const apn &C ) const
        {
            assert(C<modulus);
            const apn P = apn::mod_exp(C,publicExponent,modulus);
            if(P.bits()>decryptedBits) throw exception("%s.pub_decrypt(invalid cipher)",CLID);
            return P;
        }


    }

}
