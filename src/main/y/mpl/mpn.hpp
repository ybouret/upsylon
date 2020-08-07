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
            typedef list<const PrimeInfo> List; //!< alias
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
            virtual ~MetaPrimeVector() throw();         //!< destructor

            Slot   *slot;     //!< slot[1.size]
            size_t  size;     //!< 0..capacity
            size_t  capacity; //!< from manager
            size_t  bytes;    //!< for  allocator
            size_t  shift;    //!< for allocator
            
            void    reserve( size_t n );           //!< reserve extra memory
            void    ensure( const size_t n);       //!< ensure capacity >= n
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

        //----------------------------------------------------------------------
        //
        // simplification
        //
        //----------------------------------------------------------------------


        //! simplify a sequence of mpn/mpz
        template <typename SEQUENCE> inline
        mpn simplify( SEQUENCE &seq ) const
        {
            typedef typename SEQUENCE::mutable_type type;
            const size_t size = seq.size();
            type         smax = 0;
            
            // find the smallest not zero item
            {
                typename SEQUENCE::iterator it = seq.begin();
                size_t                      i  = size;
                for(;i>0;--i,++it)
                {
                    const type temp = mkl::fabs_of(*it);
                    if(temp>0)
                    {
                        smax = temp;
                        break;
                    }
                }
                for(;i>0;--i,++it)
                {
                    const type temp = mkl::fabs_of(*it);
                    if(temp>0&&temp<smax)
                    {
                        smax = temp;
                    }
                }
            }

            // check dividers: TODO optimize with list!
            mpn product = _1;
            for(mpn factor=_2;factor<=smax;factor = nextPrime(++factor) )
            {
                while( AllDivisibleBy(factor,seq) )
                {
                    product *= factor;
                    DivideAllBy(factor,seq);
                }
            }

            return product;
        }

    private:
        explicit MPN();
        virtual ~MPN() throw();
        friend class singleton<MPN>;
        size_t nextProbe();

    public:
        static const at_exit::longevity life_time; //!< based on manager existence

    private:
        template <typename ITERATOR> inline static
        bool AllDivisibleBy(const mpn &factor, ITERATOR curr, size_t count )
        {
            assert(factor.is_positive());
            while(count-- > 0)
            {
                if( !((*curr).is_divisible_by(factor)) ) return false;
                ++curr;
            }
            return true;
        }

        template <typename SEQUENCE> inline static
        bool AllDivisibleBy(const mpn &factor, const SEQUENCE &seq )
        {
            return AllDivisibleBy(factor,seq.begin(),seq.size());
        }

        template <typename ITERATOR> inline static
        void DivideAllBy(const mpn &factor, ITERATOR curr, size_t count )
        {
            assert(factor.is_positive());
            while(count-- > 0)
            {
                *curr /= factor;
                ++curr;
            }
        }

        template <typename SEQUENCE> inline static
        void DivideAllBy(const mpn &factor, SEQUENCE &seq)
        {
            DivideAllBy(factor,seq.begin(),seq.size());
        }

    };


}


#endif

