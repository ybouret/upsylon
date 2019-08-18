
//! \file
#ifndef Y_MPL_PRIME_FACTORS_RATIO_INCLUDED
#define Y_MPL_PRIME_FACTORS_RATIO_INCLUDED 1

#include "y/mpl/prime/factors.hpp"
#include "y/mpl/rational.hpp"

namespace upsylon
{
    namespace mpl
    {

        //! rational fraction of prime factors
        class prime_factors_ratio : public number_type
        {
        public:
            const prime_factors num; //!< numerator
            const prime_factors den; //!< denominator

            prime_factors_ratio();   //!< 0/1
            prime_factors_ratio( const prime_factors &n ); //!< n/1
            prime_factors_ratio( const prime_factors &n, const prime_factors &d ); //!< n/d

            prime_factors_ratio( const natural &n ); //!< n/1
            prime_factors_ratio( const natural &n, const natural &d); //!< n/d

            prime_factors_ratio( const word_t n); //!< n/1
            prime_factors_ratio( const word_t n, const word_t d); //!< n/d

            void xch( prime_factors_ratio &Q ) throw(); //!< no-throw swap

            prime_factors_ratio( const prime_factors_ratio & ); //!< copy
            prime_factors_ratio & operator=( const prime_factors_ratio & ); //!< assign

            virtual ~prime_factors_ratio() throw(); //!< destructor


            void mul_by( const prime_factors_ratio &x ); //!< *= x
            void div_by( const prime_factors_ratio &x ); //!< /= x


            void mul_by( const prime_factors &n ); //!< *= n
            void div_by( const prime_factors &d ); //!< /= d

            void mul_by( const natural &n ); //!< *= n
            void div_by( const natural &d ); //!< /= d

            void mul_by( const word_t n ); //!< *= n
            void div_by( const word_t d ); //!< /= d

            rational value() const; //!< convert to rational

            //! output
            friend std::ostream & operator<<( std::ostream &, const prime_factors_ratio &Q );
            
            virtual const char *className() const throw();            //!< CLASS_NAME
            virtual size_t      serialize( ios::ostream &fp ) const;  //!< number of factors and factors
            static const char   CLASS_NAME[];                         //!< "mpQ"
            static prime_factors_ratio read( ios::istream &fp);       //!< read from serialize()

            //! test for equality
            friend bool operator==( const prime_factors_ratio &, const prime_factors_ratio &) throw();
          
            //! test for difference
            friend bool operator!=( const prime_factors_ratio &, const prime_factors_ratio &) throw();


        private:
            void update();
        };

    }

    typedef mpl::prime_factors_ratio mpQ; //!< alias

}

#endif

