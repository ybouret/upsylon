

//! \file
#ifndef Y_AP_LIBRARY_INCLUDED
#define Y_AP_LIBRARY_INCLUDED 1

#include "y/yap/rational.hpp"
#include "y/yap/prime.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon {


    namespace yap {

        
        //______________________________________________________________________
        //
        //
        //! precompiled data
        //
        //______________________________________________________________________
        class library : public singleton<library>, public ios::serializable
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const char CLASS_NAME[];        //!< "yapl"
            Y_SINGLETON_DECL(library);             //!< aliases
            typedef auto_ptr<const prime> pprime;  //!< alias
            //__________________________________________________________________
            //
            // serializable
            //__________________________________________________________________
            virtual const char *className() const throw();       //!< className
            virtual size_t      serialize(ios::ostream &) const; //!< encoded


            //__________________________________________________________________
            //
            // primes handling
            //__________________________________________________________________
            void    reset_primes() throw();                   //!< clean up content
            bool    is_prime_(const natural        &n) const; //!< sieve method
            bool    is_prime_(const number::utype   u) const; //!< sieve method
            natural next_prime_(const natural      &n) const; //!< using sieve
            natural next_prime_(const number::utype u) const; //!< using sieve

            const prime & prefetch();           //!< prefetch next prime in primes list
            const prime & prefetch(size_t n);   //!< prefetch next primes in primes list


            //__________________________________________________________________
            //
            // named constants
            //__________________________________________________________________
            const natural _0;  //!< 0x00
            const natural _1;  //!< 0x01
            const natural _2;  //!< 0x02
            const natural _3;  //!< 0x03
            const natural _4;  //!< 0x04
            const natural _5;  //!< 0x05
            const natural _6;  //!< 0x06
            const natural _7;  //!< 0x07
            const natural _8;  //!< 0x08
            const natural _9;  //!< 0x09
            const natural _10; //!< 0x0a
            const natural _11; //!< 0x0b
            const natural _12; //!< 0x0c
            const natural _13; //!< 0x0d
            const natural _14; //!< 0x0e
            const natural _15; //!< 0x0f
            const natural _16; //!< 0x10

            const integer  _z0; //!< int(0)
            const rational _q0; //!< (0/1)

            //__________________________________________________________________
            //
            // primes db
            //__________________________________________________________________
            const natural         &pstart; //!< _5
            const prime::list_type primes; //!< list of precomputed primes
            const natural          launch; //!< 5 or next multiple of 6 above last prime
            const pprime           p2;     //!< 2 as prime
            const pprime           p3;     //!< 3 as prime
            const prime           &upper() const throw(); //!< upper prime
            bool                   prune();               //!< try to prune last prime
            size_t                 size()  const throw(); //!< 2+primes.count

        private:
            Y_DISABLE_COPY_AND_ASSIGN(library);
            explicit library();
            virtual ~library() throw();


        };

    }

}

#endif
