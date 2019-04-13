//! \file
#ifndef Y_MPN_INCLUDED
#define Y_MPN_INCLUDED 1

#include "y/mpl/natural.hpp"
#include "y/hashing/sha1.hpp"
#include "y/sequence/list.hpp"

namespace upsylon
{


    //! precompiled naturals
    class MPN : public singleton<MPN>, public ios::serializable
    {
    public:
        //----------------------------------------------------------------------
        //
        // data types
        //
        //----------------------------------------------------------------------
        static const char CLASS_NAME[]; //!< "MPN" for serialize
        typedef hashing::sha1 Hasher;   //!< internal hasher type

        //! store prime and its squared value
        class PrimeInfo
        {
        public:
            typedef list<const PrimeInfo> List; //!< alis
            PrimeInfo(const mpn &n);            //!< setup
            PrimeInfo(const uint64_t n);        //!< setup
            PrimeInfo(const PrimeInfo &);       //!< hard copy
            ~PrimeInfo() throw();               //!< destructor
            const mpn p;                        //!< a prime
            const mpn q;                        //!< p^2

            //! output just the prime value
            friend std::ostream & operator<<( std::ostream &os, const PrimeInfo &);

        private:
            Y_DISABLE_ASSIGN(PrimeInfo);
        };

        //! handle PrimeInfo
        class ListOfPrimeInfo : public PrimeInfo::List
        {
        public:
            explicit ListOfPrimeInfo() throw();        //!< default setup
            explicit ListOfPrimeInfo(const size_t n);  //!< reserve memory
            virtual ~ListOfPrimeInfo() throw();        //!< destructor
            const mpn & lower() const throw();         //!< front().p
            const mpn & upper() const throw();         //!< back().p

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ListOfPrimeInfo);
        };

        //! holds address of computed primes
        class MetaPrimeVector
        {
        public:
            typedef const mpn *Slot; //!< alias

            explicit MetaPrimeVector(const size_t n=0); //!< setup
            virtual ~MetaPrimeVector() throw();         //!< destryuctor

            Slot   *slot;     //!< slot[1.size]
            size_t  size;     //!< 0..capacity
            size_t  capacity; //!< from manager
            size_t  bytes;    //!< for  allocator

            void    reserve( size_t n );           //!< reserve extra memory
            void    record( const mpn &prime_ref); //!< add prime reference

        private:
            Y_DISABLE_COPY_AND_ASSIGN(MetaPrimeVector);
        };

        //----------------------------------------------------------------------
        //
        // public data
        //
        //----------------------------------------------------------------------


        const ListOfPrimeInfo plist;  //!< precomputed primes, with at least 2 and 3
        const MetaPrimeVector mpvec;  //!< from plist
        const mpn             probe;  //!< next prime probe 5+k*6>=highest prime
        const mpn            _0;      //!< 0
        const mpn            _1;      //!< 1
        const mpn            _2;      //!< 2
        const mpn            _3;      //!< 3
        const mpn            _4;      //!< 4
        const mpn            _5;      //!< 5
        const mpn            _6;      //!< 6
        const mpn            _10;     //!< 10


        //----------------------------------------------------------------------
        //
        // serializable interface
        //
        //----------------------------------------------------------------------
        virtual const char *className() const throw();           //!< CLASS_NAME
        virtual size_t      serialize( ios::ostream &fp ) const; //!< serialize primes

        //----------------------------------------------------------------------
        //
        // primes creation
        //
        //----------------------------------------------------------------------
        void reset() throw();                    //!< reset list and probe (failsafe)
        void reloadPrimes( ios::istream &fp);    //!< try to reload
        void createPrimes( const size_t count ); //!< append count primes to the primes sequence

        //----------------------------------------------------------------------
        //
        // primes access
        //
        //----------------------------------------------------------------------
        bool       isPrime_( const mpn &n )   const;  //!< raw method
        bool       isPrime(  const mpn &n )   const;  //!< hybrid method
        mpn        nextPrime_( const mpn &n ) const;  //!< raw method
        mpn        nextPrime(  const mpn &n ) const;  //!< hybrid method

        digest md() const;      //!< current digest

        bool locateNextPrime(mpn       &n) const; //!< using primes list
        bool isComputedPrime(const mpn &n) const; //!< in primes list

    private:
        explicit MPN();
        virtual ~MPN() throw();
        friend class singleton<MPN>;
        size_t nextProbe();

    public:
        static const at_exit::longevity life_time = mpl::manager::life_time - 1; //!< based on manager existence
    };

}


#endif

