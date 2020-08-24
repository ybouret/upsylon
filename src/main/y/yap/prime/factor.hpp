//! \file

#ifndef Y_AP_PRIME_FACTOR_INCLUDED
#define Y_AP_PRIME_FACTOR_INCLUDED 1

#include "y/yap/natural.hpp"

namespace upsylon {

    namespace yap {

        //______________________________________________________________________
        //
        //
        //! a prime factor
        //
        //______________________________________________________________________
        class prime_factor : public number
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const char CLASS_NAME[];

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit prime_factor(const natural &prm, const size_t xpn);
            explicit prime_factor(const utype    prm, const size_t xpn);
            virtual ~prime_factor() throw();
            prime_factor(const prime_factor &) throw();
            prime_factor & operator=(const prime_factor &) throw();

            prime_factor *next;
            const natural p;
            const size_t  n;

            //__________________________________________________________________
            //
            // serializable
            //__________________________________________________________________
            virtual const char   *className() const throw();
            virtual size_t        serialize(ios::ostream &) const;
            static  prime_factor  read(ios::istream &, size_t &, const char *); //!< relaod

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            natural value() const;
            friend bool operator==(const prime_factor &lhs, const prime_factor &rhs) throw();
            friend bool operator!=(const prime_factor &lhs, const prime_factor &rhs) throw();
            friend std::ostream & operator<<( std::ostream &os, const prime_factor &f);

        private:
            void setup() throw();
        };

    }
}

#endif
