//! \file
#ifndef Y_MPL_RSA_KEY_INCLUDED
#define Y_MPL_RSA_KEY_INCLUDED 1

#include "y/mpl/natural.hpp"
#include "y/ptr/arc.hpp"

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
            
            virtual Key * clone()                 const = 0;
            virtual void  print(std::ostream &os) const = 0;

        protected:
            explicit Key(const mpn &m );
            explicit Key( const Key &other );
            
        private:
            Y_DISABLE_ASSIGN(Key);
        };

        typedef arc_ptr<Key> SharedKey;
        
        class PublicKey : public Key
        {
        public:
            const mpn publicExponent;
            
            virtual ~PublicKey() throw();
            explicit PublicKey( const mpn &m, const mpn &e );
            explicit PublicKey( const PublicKey &other );
            
            virtual Key  *clone()                   const;
            virtual void  print( std::ostream &os ) const;

            static  Key *Create(const mpn &p, const mpn &q, const mpn &e);

        private:
            Y_DISABLE_ASSIGN(PublicKey);
        };



        class PrivateKey : public Key
        {
        public:
            const mpn publicExponent;
            const mpn privateExponent;
            const mpn prime1;
            const mpn prime2;
            const mpn exponent1;
            const mpn exponent2;
            const mpn coefficient;

            virtual ~PrivateKey() throw();
            explicit PrivateKey(const mpn &m,
                                const mpn &e,
                                const mpn &d,
                                const mpn &p1,
                                const mpn &p2,
                                const mpn &e1,
                                const mpn &e2,
                                const mpn &c);

            explicit PrivateKey( const PrivateKey &other );

            virtual Key  *clone()                   const;
            virtual void  print( std::ostream &os ) const;

            static  Key *Create(const mpn &p, const mpn &q, const mpn &e);


        private:
            Y_DISABLE_ASSIGN(PrivateKey);
        };
        
    }
}

#endif


