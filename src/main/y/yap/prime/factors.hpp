
//! \file

#ifndef Y_AP_PRIME_FACTORS_INCLUDED
#define Y_AP_PRIME_FACTORS_INCLUDED 1

#include "y/yap/prime/factor.hpp"
#include "y/core/roll.hpp"

namespace upsylon {

    namespace yap {

        //______________________________________________________________________
        //
        //
        //! a prime factors decomposition
        //
        //______________________________________________________________________
        class prime_factors : public number
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const char CLASS_NAME[]; //!< "yapF";
            typedef core::roll_of_cpp<prime_factor> factors_type; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            prime_factors() throw();                           //!< zero
            prime_factors(const natural &n);                   //!< decompose
            prime_factors(const prime_factors &);              //!< copy
            prime_factors & operator=( const prime_factors &); //!< assign
            virtual ~prime_factors() throw();                  //!< cleanup

            friend std::ostream & operator<<(std::ostream &os, const prime_factors &F);

            //__________________________________________________________________
            //
            // serialize
            //__________________________________________________________________
            virtual const char *className() const throw();       //!< CLASS_NAME
            virtual size_t      serialize(ios::ostream &) const; //!< binary save

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            natural value() const;

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const factors_type factors;
        };

    }

}

#endif
