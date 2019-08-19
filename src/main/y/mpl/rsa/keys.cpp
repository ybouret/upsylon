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
    }
    
}

