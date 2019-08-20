#include "y/mpl/rsa/keys.hpp"

namespace upsylon
{
    namespace RSA
    {
        
        Key:: ~Key() throw()
        {
        }
        
        
        static inline
        size_t maxbits_for(const mpn &modulus )
        {
            const mpn m = modulus.__dec();
            return m.bits();
        }
        
        Key:: Key( const mpn &m) :
        modulus(m),
        maxbits( maxbits_for(modulus) )
        {
            
        }
        
        Key:: Key( const Key &other ) :
        modulus( other.modulus ),
        maxbits( other.maxbits )
        {
            
        }
        
    }
}

namespace upsylon
{
    namespace RSA
    {
        PublicKey:: PublicKey( const mpn &m, const mpn &e ) :
        Key(m),
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

        Key * PublicKey:: Create(const mpn &p, const mpn &q, const mpn &e)
        {
            mpn       E = e;
            const mpn M = p*q;
            return new PublicKey(M,E);
        }
    }
    
}


#include "y/exception.hpp"

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
        Key(m),
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

    }

}

