//! \file
#ifndef Y_MPL_PRIME_FACTORS_INCLUDED
#define Y_MPL_PRIME_FACTORS_INCLUDED 1

#include "y/mpl/prime/factor.hpp"

namespace upsylon
{
    namespace mpl
    {
        //! unique prime factor decomposition
        class prime_factors : public number_type
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


            void mul_by( const prime_factors &other ); //!< multiplication
            void mul_by( const natural       &other ); //!< multiplication
            void mul_by( const word_t         other ); //!< multiplication




            size_t         count() const throw(); //!< factors.size()
            const_iterator begin() const throw(); //!< factors.begin()
            const_iterator end()   const throw(); //!< factors.end()

            bool is_zero() const throw(); //!< check if 0
            bool is_one()  const throw(); //!< check if 1

            void xch( prime_factors &other ) throw(); //!< no throw swap

            //! factorial
            static prime_factors factorial(const natural &n);
            static prime_factors perm(const natural &n, const natural &k);
            static prime_factors comb(const natural &n, const natural &k);
            
        private:
            prime_factor::db factors;
            void update();
            void setup( const natural & );
            void __add( const natural &p, const size_t n );
            void __add_safe( const natural &p, const size_t n);

            friend class prime_factors_ratio;
        };
    }

}


#endif


