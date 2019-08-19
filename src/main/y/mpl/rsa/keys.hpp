//! \file
#ifndef Y_MPL_RSA_KEY_INCLUDED
#define Y_MPL_RSA_KEY_INCLUDED 1

#include "y/mpl/natural.hpp"

namespace upsylon
{
    namespace RSA
    {
        //! interface for a key
        class Key : public counted_object
        {
        public:
            virtual ~Key() throw();
            
            const mpn    modulus;
            const mpn    maxbits;
            
            virtual Key * clone() const = 0;
            
        protected:
            explicit Key(const mpn &m );
            explicit Key( const Key &other );
            
        private:
            Y_DISABLE_ASSIGN(Key);
        };
        
        class PublicKey : public Key
        {
        public:
            const mpn publicExponent;
            
            virtual ~PublicKey() throw();
            explicit PublicKey( const mpn &m, const mpn &e );
            explicit PublicKey( const PublicKey &other );
            
            virtual Key *clone() const;
            
        private:
            Y_DISABLE_ASSIGN(PublicKey);
        };
        
        class PrivateKey : public Key
        {
        public:
            const mpn privateExponent;
            
            virtual ~PrivateKey() throw();
            explicit PrivateKey( const mpn &m, const mpn &d);
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(PrivateKey);
        };
        
    }
}

#endif


