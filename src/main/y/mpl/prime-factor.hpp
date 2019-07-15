//! \file
#ifndef Y_MPL_PRIME_FACTOR_INCLUDED
#define Y_MPL_PRIME_FACTOR_INCLUDED 1

#include "y/mpl/natural.hpp"

#include "y/memory/pooled.hpp"
#include "y/associative/set.hpp"
#include "y/ptr/intr.hpp"

namespace upsylon
{
    namespace mpl
    {

        //! a prime factor p^n
        class prime_factor : public counted_object
        {
        public:
            //------------------------------------------------------------------
            //
            // types
            //
            //------------------------------------------------------------------
            typedef intr_ptr<const natural,prime_factor>                     pointer; //!< shared pointer
            typedef memory::pooled                                           memType; //!< medium sized factors
            typedef hashing::fnv                                             hfnType; //!< hashing
            typedef key_hasher<const natural,hfnType>                        pkhType; //!< prime key hasher
            typedef set<const natural,prime_factor::pointer,pkhType,memType> db;      //!< database


            //------------------------------------------------------------------
            //
            // methods
            //
            //------------------------------------------------------------------
            prime_factor( const natural &prm, const size_t num ); //!< setup
            prime_factor( const prime_factor &);                  //!< copy
            virtual ~prime_factor() throw();                      //!< cleanup
            const natural & key() const throw();                  //!< for database
            natural       get() const;                            //!< compute product

            //! output
            friend std::ostream & operator<<( std::ostream &os, const prime_factor & );

            //! comparing keys to keep databases ordered
            static int compare_keys( const natural &lhs, const natural &rhs ) throw();

            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            const natural p; //!< prime
            size_t        n; //!< count > 0


        private:
            Y_DISABLE_ASSIGN(prime_factor);
        };


        //! unique prime factor decomposition
        class prime_factors : public counted_object
        {
        public:
            typedef prime_factor::db::const_iterator const_iterator; //!< alias

            virtual ~prime_factors() throw();                         //!< cleanup
            prime_factors() throw();                                  //!< would be 0
            prime_factors(const prime_factors &other);                //!< copy
            prime_factors & operator=( const prime_factors &other );  //!< assign

            prime_factors( const natural &x ); //!< setup from mpn
            prime_factors( const word_t   x ); //!< setup from integral typ
            prime_factors( const char    *s ); //!< setup from text
            prime_factors( const string  &s ); //!< setup from string

            //! output
            friend std::ostream & operator<<( std::ostream &, const prime_factors &pfs );

            //! compute
            natural get() const;

            //! multiplication
            void mul_by( const prime_factors &other );

            //! factorial
            static prime_factors factorial(const natural &n);

        private:
            prime_factor::db factors;
            void update();
            void setup( const natural & );
            void __add( const natural &p, const size_t n);

        };

    }
}


#endif
