
//! \file
#ifndef Y_MPL_PRIME_FACTORS_RATIO_INCLUDED
#define Y_MPL_PRIME_FACTORS_RATIO_INCLUDED 1

#include "y/mpl/prime/factors.hpp"

namespace upsylon
{
    namespace mpl
    {

        class prime_factors_ratio : public number_type
        {
        public:
            const prime_factors num;
            const prime_factors den;

            prime_factors_ratio();
            prime_factors_ratio( const prime_factors &n );
            prime_factors_ratio( const prime_factors &n, const prime_factors &d );

            prime_factors_ratio( const natural &n );
            prime_factors_ratio( const natural &n, const natural &d);

            prime_factors_ratio( const word_t n);
            prime_factors_ratio( const word_t n, const word_t d);

            void xch( prime_factors_ratio &Q ) throw();

            prime_factors_ratio( const prime_factors_ratio & );
            prime_factors_ratio & operator=( const prime_factors_ratio & );

            virtual ~prime_factors_ratio() throw();

            friend std::ostream & operator<<( std::ostream &, const prime_factors_ratio &Q );

            void mul_by( const prime_factors_ratio &x );
            void div_by( const prime_factors_ratio &x );


            void mul_by( const prime_factors &n );
            void div_by( const prime_factors &d );

            void mul_by( const natural &n );
            void div_by( const natural &d );

            void mul_by( const word_t n );
            void div_by( const word_t d );


        private:
            void update();
        };

    }

}

#endif

