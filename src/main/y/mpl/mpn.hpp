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
        static const char CLASS_NAME[]; //!< "MPN"

        //! used for list creation
        enum CreateMode
        {
            CreateSafe, //!< always use isPrime_
            CreateFast  //!< use isPrime from freshly computed list
        };

        typedef hashing::sha1 Hasher; //!< internal hasher

        //! store prime and its squared value
        class PrimeInfo
        {
        public:
            PrimeInfo(const mpn &n);      //!< setup
            PrimeInfo(const PrimeInfo &); //!< hard copy
            ~PrimeInfo() throw();         //!< destructor
            const mpn p;                  //!< a prime
            const mpn q;                  //!< p^2

            //! output just the value
            friend std::ostream & operator<<( std::ostream &os, const PrimeInfo &);

        private:
            Y_DISABLE_ASSIGN(PrimeInfo);
        };

        typedef list<const PrimeInfo> PrimeList; //!< alias
        const PrimeList plist;  //!< precomputed primes, with at least 2 and 3
        const mpn       probe;  //!< next prime probe 5+k*6>=highest prime
        const mpn      _0;      //!< 0
        const mpn      _1;      //!< 1
        const mpn      _2;      //!< 2
        const mpn      _3;      //!< 3
        const mpn      _4;      //!< 4
        const mpn      _5;      //!< 5
        const mpn      _6;      //!< 6
        const mpn      _10;     //!< 10

        virtual const char *className() const throw();           //!< CLASS_NAME
        virtual size_t      serialize( ios::ostream &fp ) const; //!< serialize primes

        void    reloadPrimes( ios::istream &fp);        //!< try to reload
        void    createPrimes( const size_t count,const CreateMode how=CreateSafe ); //!< append count primes to the primes sequence

        bool isPrime_( const mpn &n ) const;   //!< raw method
        bool isPrime(  const mpn &n ) const;   //!< hybrid method
        mpn  nextPrime_( const mpn &n ) const; //!< raw method
        mpn  nextPrime(  const mpn &n ) const; //!< hybrid method

        void   reset() throw(); //!< reset list and probe (failsafe)
        digest md() const;      //!< current digest

    private:
        explicit MPN();
        virtual ~MPN() throw();
        friend class singleton<MPN>;
        void   checkList() const;
        size_t nextProbe(const CreateMode how);

    public:
        static const at_exit::longevity life_time = mpl::manager::life_time - 1; //!< based on manager existence
    };

}


#endif

