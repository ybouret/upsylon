#include "y/mpl/rsa/keys.hpp"
#include "y/exception.hpp"
#include "y/hashing/function.hpp"

namespace upsylon
{
    namespace RSA
    {
        
        Key:: ~Key() throw()
        {
        }
        

        static inline
        size_t maxbits_for(const mpn &m )
        {
            const size_t mbits = m.bits();
            if(mbits<=1) throw exception("RSA::Key(modulus too small!)");
            return mbits-1;
        }

        Key:: Key( const mpn &m, const uint32_t t) :
        modulus(m),
        maximum( modulus.__dec() ),
        maxbits( maxbits_for(maximum) ),
        type(t)
        {
            assert(Private==type||Public==type);
        }
        
        Key:: Key( const Key &other ) :
        counted_object(),
        ios::serializable(),
        modulus( other.modulus ),
        maximum( other.maximum ),
        maxbits( other.maxbits ),
        type( other.type )
        {
            
        }

        const mpn & Key:: check( const mpn &M ) const
        {
            if(M>maximum) throw exception("RSA::Key(message overflow)");
            return M;
        }

        mpn Key:: __pub( const mpn &M, const mpn &publicExponent ) const
        {
            return mpn::mod_exp( check(M),publicExponent,modulus);
        }

        

        digest  Key:: md( hashing::function &H ) const
        {
            H.set();
            H(modulus);
            H(maximum);
            H.run_type(maxbits);
            H.run_type(type);
            runHash(H);
            return H.md();
        }

    }
}


#include "y/ios/ostream.hpp"

namespace upsylon
{
    namespace RSA
    {
        PublicKey:: PublicKey( const mpn &m, const mpn &e ) :
        Key(m,Public),
        publicExponent(e)
        {
        }
        
        PublicKey:: ~PublicKey() throw()
        {
        }
        
        PublicKey:: PublicKey( const PublicKey &other ) :
        Key(other),
        publicExponent( other.publicExponent )
        {
        }
        
        Key * PublicKey:: clone() const
        {
            return new PublicKey(*this);
        }

        Key * PublicKey:: clonePublic() const
        {
            return new PublicKey(*this);
        }

        Key * PublicKey:: Create(const mpn &p, const mpn &q, const mpn &e)
        {
            mpn       E = e;
            const mpn M = p*q;
            return new PublicKey(M,E);
        }

        void PublicKey:: print( std::ostream &os ) const
        {
            os << "modulus         : " << modulus        << std::endl;
            os << "publicExponent  : " << publicExponent << std::endl;
        }


        mpn   PublicKey:: pub( const mpn &M ) const
        {
            return __pub(M,publicExponent);
        }

        mpn   PublicKey:: prv( const mpn & ) const
        {
            throw exception("No RSA::PublicKey::prv" );
        }

        mpn   PublicKey:: prv_( const mpn &_ ) const
        {
            return prv(_);
        }

        const char  PublicKey:: CLASS_NAME[] = "RSAPublicKey";
        const char *PublicKey:: className() const throw() { return CLASS_NAME; }

#define Y_RSA_SRZ(FIELD) total += FIELD.serialize(fp)

        size_t PublicKey:: serialize(ios::ostream &fp) const
        {
            size_t total = fp.write_nbo(type);
            Y_RSA_SRZ(modulus);
            Y_RSA_SRZ(publicExponent);
            return total;
        }


        void PublicKey:: runHash(hashing::function &H) const throw()
        {
            H(publicExponent);
        }
    }
    
}



namespace upsylon
{
    namespace RSA
    {

        PrivateKey:: PrivateKey(const mpn &m,
                                const mpn &e,
                                const mpn &d,
                                const mpn &p1,
                                const mpn &p2,
                                const mpn &e1,
                                const mpn &e2,
                                const mpn &c) :
        Key(m,Private),
        publicExponent(e),
        privateExponent(d),
        prime1(p1),
        prime2(p2),
        exponent1(e1),
        exponent2(e2),
        coefficient(c)
        {
        }


        PrivateKey:: PrivateKey( const PrivateKey &other ) :
        Key(other),
        publicExponent ( other.publicExponent  ),
        privateExponent( other.privateExponent ),
        prime1         ( other.prime1          ),
        prime2         ( other.prime2          ),
        exponent1      ( other.exponent1       ),
        exponent2      ( other.exponent2       ),
        coefficient    ( other.coefficient     )
        {

        }

        PrivateKey:: ~PrivateKey() throw()
        {
        }

        Key * PrivateKey:: clone() const
        {
            return new PrivateKey( *this );
        }


        Key * PrivateKey:: clonePublic() const
        {
            return new PublicKey(modulus,publicExponent);
        }

        Key * PrivateKey:: Create(const mpn &p,
                                  const mpn &q,
                                  const mpn &e)
        {
            mpn P = p;
            mpn Q = q;
            if(P==Q) throw exception("RSA:PrivateKey(same primes)");
            if(P<Q) P.xch(Q); assert(P>Q);
            const mpn M   = P*Q;
            mpn       E   = e;
            const mpn PM1 = P.__dec();
            const mpn QM1 = Q.__dec();
            const mpn L   = PM1*QM1;
            const mpn D   = mpn::mod_inv(E,L);
            const mpn E1  = mpn::mod_inv(E,PM1);
            const mpn E2  = mpn::mod_inv(E,QM1);
            const mpn CF  = mpn::mod_inv(Q,P);
            
            return new PrivateKey(M,E,D,P,Q,E1,E2,CF);
        }

        void PrivateKey:: print( std::ostream &os ) const
        {
            os << "modulus         : " << modulus         << std::endl;
            os << "publicExponent  : " << publicExponent  << std::endl;
            os << "privateExponent : " << privateExponent << std::endl;
            os << "prime1          : " << prime1          << std::endl;
            os << "prime2          : " << prime2          << std::endl;
            os << "exponent1       : " << exponent1       << std::endl;
            os << "exponent2       : " << exponent2       << std::endl;
            os << "coefficient     : " << coefficient     << std::endl;

        }

        mpn   PrivateKey:: pub( const mpn &M ) const
        {
            return __pub(M,publicExponent);
        }


        mpn   PrivateKey:: prv_( const mpn &M ) const
        {
            return mpn::mod_exp( check(M),privateExponent,modulus);
        }

        mpn   PrivateKey:: prv( const mpn &C ) const
        {
            mpn       M1 = mpn::mod_exp(C,exponent1,prime1);
            const mpn M2 = mpn::mod_exp(C,exponent2,prime2);
            while(M1<M2) M1 += prime1;
            const mpn H  = coefficient*(M1-M2);
            const mpn h  = mpn::__mod(H,prime1);
            return M2 + h*prime2;
        }


        const char  PrivateKey:: CLASS_NAME[] = "RSAPrivateKey";
        const char *PrivateKey:: className() const throw() { return CLASS_NAME; }

        size_t PrivateKey:: serialize(ios::ostream &fp) const
        {
            size_t total = fp.write_nbo(type);
            Y_RSA_SRZ(modulus);
            Y_RSA_SRZ(publicExponent);
            Y_RSA_SRZ(privateExponent);
            Y_RSA_SRZ(prime1);
            Y_RSA_SRZ(prime2);
            Y_RSA_SRZ(exponent1);
            Y_RSA_SRZ(exponent2);
            Y_RSA_SRZ(coefficient);
            return total;
        }

        void PrivateKey:: runHash(hashing::function &H) const throw()
        {
            H(publicExponent);
            H(privateExponent);
            H(prime1);
            H(prime2);
            H(exponent1);
            H(exponent2);
            H(coefficient);
        }
    }

}

#include "y/string.hpp"

namespace upsylon
{
    namespace RSA
    {
        Key * Key:: Read( ios::istream &fp, size_t *shift, const ReadMode readMode )
        {
            uint32_t       tag = 0;
            if(!fp.query_nbo(tag)) throw exception("RSA::Key::Read(missing tag)");
            size_t         num = sizeof(uint32_t);
            size_t         tmp = 0;
            switch(tag)
            {
                    //__________________________________________________________
                    //
                    // read a public key
                    //__________________________________________________________
                case Key::Public: {
                    const mpn m = mpn::read(fp,&tmp,"rsa_public_key.m"); num += tmp;
                    const mpn e = mpn::read(fp,&tmp,"rsa_public_key.m"); num += tmp;
                    if(shift)
                    {
                        *shift = num;
                    }
                    return new PublicKey(m,e);
                }

                case Key::Private:
                {
                    const mpn m  = mpn::read(fp,&tmp,"rsa_public_key.m");  num += tmp;
                    const mpn e  = mpn::read(fp,&tmp,"rsa_public_key.e");  num += tmp;
                    const mpn d  = mpn::read(fp,&tmp,"rsa_public_key.d");  num += tmp;
                    const mpn p1 = mpn::read(fp,&tmp,"rsa_public_key.p1"); num += tmp;
                    const mpn p2 = mpn::read(fp,&tmp,"rsa_public_key.p2"); num += tmp;
                    const mpn e1 = mpn::read(fp,&tmp,"rsa_public_key.e1"); num += tmp;
                    const mpn e2 = mpn::read(fp,&tmp,"rsa_public_key.e2"); num += tmp;
                    const mpn cf = mpn::read(fp,&tmp,"rsa_public_key.cf"); num += tmp;
                    if(shift)
                    {
                        *shift = num;
                    }
                    switch(readMode)
                    {
                        case ReadDefault:    return new PrivateKey(m,e,d,p1,p2,e1,e2,cf);
                        case ReadPublicOnly: return new PublicKey(m,e);
                    }
                }
                default:
                    break;
            }
            throw exception("RSA.Key.read(invalid type)");
            //return NULL;
        }
    }
}
