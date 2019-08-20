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
            static const uint32_t Public  = Y_FOURCC('@','P','U','B');
            static const uint32_t Private = Y_FOURCC('@','P','R','V');

            virtual ~Key() throw();

            //! data
            const mpn      modulus;
            const mpn      maximum;
            const size_t   maxbits;
            const uint32_t type;

            //------------------------------------------------------------------
            // virtual interface
            //------------------------------------------------------------------
            virtual Key * clone()                 const = 0;
            virtual Key * clonePublic()           const = 0;
            virtual void  print(std::ostream &os) const = 0;
            virtual mpn   pub( const mpn & )      const = 0;
            virtual mpn   prv( const mpn & )      const = 0;
            virtual mpn   prv_( const mpn & )     const = 0;

            //------------------------------------------------------------------
            // non virtual interface
            //------------------------------------------------------------------
            digest  md( hashing::function &H ) const;


            //------------------------------------------------------------------
            // common operations
            //------------------------------------------------------------------
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
            static Key * Read( ios::istream &fp, const ReadMode readMode = ReadDefault); //!< reload key


        protected:
            explicit Key(const mpn &m, const uint32_t t);
            explicit Key(const Key &other );
            virtual void runHash(  hashing::function &H ) const throw() = 0;


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

            //------------------------------------------------------------------
            // virtual interface
            //------------------------------------------------------------------
            virtual Key  *clone()                   const;
            virtual Key * clonePublic()             const;
            virtual void  print( std::ostream &os ) const;
            virtual mpn   pub( const mpn &X )       const;
            virtual mpn   prv( const mpn & )        const; //!< exception!
            virtual mpn   prv_( const mpn & )       const; //!< exception!


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

            //------------------------------------------------------------------
            // Key interface
            //------------------------------------------------------------------
            virtual Key  *clone()                   const;
            virtual Key * clonePublic()             const;
            virtual void  print( std::ostream &os ) const;
            virtual mpn   pub( const mpn &X )       const;
            virtual mpn   prv( const mpn & )        const; //!< use CRT
            virtual mpn   prv_( const mpn & )       const; //!< direct transform
            static  Key *Create(const mpn &p, const mpn &q, const mpn &e);

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


