//! \file
#ifndef Y_MPL_RSA_KEY_INCLUDED
#define Y_MPL_RSA_KEY_INCLUDED 1

#include "y/mpl/natural.hpp"
#include "y/ptr/arc.hpp"
#include "y/type/fourcc.hpp"
#include "y/hashing/function.hpp"

namespace upsylon
{
    namespace RSA
    {
        //! interface for a key
        class Key : public counted_object, public ios::serializable
        {
        public:
            static const uint32_t Public  = Y_FOURCC('@','P','U','B'); //!< tag for Public
            static const uint32_t Private = Y_FOURCC('@','P','R','V'); //!< tag for Privat

            //------------------------------------------------------------------
            // common data
            //------------------------------------------------------------------
            const mpn      modulus; //!< product of two primes
            const mpn      maximum; //!< modulus-1
            const size_t   maxbits; //!< maximum.bits()-1
            const uint32_t type;    //!< [Public|Private]

            //------------------------------------------------------------------
            // virtual interface
            //------------------------------------------------------------------
            virtual      ~Key() throw();                      //!< destructor
            virtual Key * clone()                 const = 0;  //!< full clone
            virtual Key * clonePublic()           const = 0;  //!< make a public key
            virtual void  print(std::ostream &os) const = 0;  //!< display content
            virtual mpn   pub( const mpn & )      const = 0;  //!< encode using public part
            virtual mpn   prv( const mpn & )      const = 0;  //!< encode using private part, optimized
            virtual mpn   prv_( const mpn & )     const = 0;  //!< raw encode using private part

            //------------------------------------------------------------------
            // non virtual interface
            //------------------------------------------------------------------
            digest      md( hashing::function &H ) const;                       //!< get a message digest of the full key
            mpn         __pub( const mpn &M, const mpn &publicExponent ) const; //!< M^publicExponent [modulus]
            const mpn & check( const mpn &M) const;                             //!< M<=maximum


            //------------------------------------------------------------------
            // reloading
            //------------------------------------------------------------------
            //! how to read
            enum ReadMode
            {
                ReadDefault,    //!< any key
                ReadPublicOnly  //!< Public part only
            };
            static Key * Read( ios::istream &fp, size_t &shift, const ReadMode readMode = ReadDefault); //!< reload key


        protected:
            explicit Key(const mpn &m, const uint32_t t); //!< initialize modulus and type
            explicit Key(const Key &other );              //!< copy common part

        private:
            virtual void runHash(  hashing::function &H ) const throw() = 0;
            Y_DISABLE_ASSIGN(Key);
        };

        //! alias to handle dynamic keys
        typedef arc_ptr<Key> SharedKey;

        //! RSA Public Key implementation
        class PublicKey : public Key
        {
        public:
            const mpn publicExponent; //!< public Exponent
            
            explicit PublicKey( const mpn &m, const mpn &e ); //!< constructor
            explicit PublicKey( const PublicKey &other );     //!< full copy

            //------------------------------------------------------------------
            // virtual interface
            //------------------------------------------------------------------
            virtual      ~PublicKey() throw();             //!< destructor
            virtual Key  *clone()                   const; //!< self copy
            virtual Key * clonePublic()             const; //!< self copy
            virtual void  print( std::ostream &os ) const; //!< print
            virtual mpn   pub( const mpn &M )       const; //!< M^publicExponent [modulus]
            virtual mpn   prv( const mpn &  )       const; //!< exception!
            virtual mpn   prv_( const mpn & )       const; //!< exception!

            //! modulus=p*q, publicExponent = e
            static  Key *Create(const mpn &p, const mpn &q, const mpn &e);

            //------------------------------------------------------------------
            // I/O interface
            //-----------------------------------------------------------------_
            static  const char   CLASS_NAME[];                        //!< "RSAPublicKey"
            virtual const char * className() const throw();           //!<  CLASS_NAME
            virtual size_t       serialize( ios::ostream &fp ) const; //!< tag+data


        private:
            Y_DISABLE_ASSIGN(PublicKey);
            virtual void runHash( hashing::function &H ) const throw();
        };


        //! RSA Private Key implementation
        class PrivateKey : public Key
        {
        public:
            const mpn publicExponent;      //!< public exponent
            const mpn privateExponent;     //!< private exponent
            const mpn prime1;              //!< first prime  prime1>prime2
            const mpn prime2;              //!< second prime prime2<prime1
            const mpn exponent1;           //!< (1/e) [prime1-1]
            const mpn exponent2;           //!< (1/e) [prime2-1]
            const mpn coefficient;         //!< (1/prime2) [prime1]

            //! constructor
            explicit PrivateKey(const mpn &m,
                                const mpn &e,
                                const mpn &d,
                                const mpn &p1,
                                const mpn &p2,
                                const mpn &e1,
                                const mpn &e2,
                                const mpn &c);

            //! full copy
            explicit PrivateKey( const PrivateKey &other );

            //------------------------------------------------------------------
            // Key interface
            //------------------------------------------------------------------
            virtual      ~PrivateKey() throw();              //!< destructor
            virtual Key  *clone()                   const;   //!< self clone
            virtual Key * clonePublic()             const;   //!< make a public key
            virtual void  print( std::ostream &os ) const;   //!< print
            virtual mpn   pub( const mpn &M )       const;   //!< C^e [modulus]
            virtual mpn   prv( const mpn &C )        const;  //!< use CRT to compute C^d [modulus]
            virtual mpn   prv_(const mpn &C )       const;   //!< C^d [modulus]

            //! compute key according to formula
            static  Key  *Create(const mpn &p, const mpn &q, const mpn &e);

            //------------------------------------------------------------------
            // I/O interface
            //-----------------------------------------------------------------_
            static  const char   CLASS_NAME[];                        //!< "RSAPrivateKey"
            virtual const char * className() const throw();           //!<  CLASS_NAME
            virtual size_t       serialize( ios::ostream &fp ) const; //!< tag+data


        private:
            Y_DISABLE_ASSIGN(PrivateKey);
            virtual void runHash( hashing::function &H ) const throw();
        };
        
    }
}

#endif


