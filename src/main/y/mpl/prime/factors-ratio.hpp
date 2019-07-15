
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


            virtual ~prime_factors_ratio() throw();

            friend std::ostream & operator<<( std::ostream &, const prime_factors_ratio &Q );

        private:
            void update();
        };

    }

}

#endif

