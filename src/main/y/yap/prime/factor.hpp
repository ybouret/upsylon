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
            static const char CLASS_NAME[]; //!< "yapf"

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit prime_factor(const natural &prm, const size_t xpn); //!< setup
            explicit prime_factor(const utype    prm, const size_t xpn); //!< setyp
            virtual ~prime_factor() throw();                             //!< cleanup
            prime_factor(const prime_factor &) throw();                  //!< copy
            prime_factor & operator=(const prime_factor &) throw();      //!< assing

            prime_factor *next; //!< for pool/roll
            const natural p;    //!< prime number
            const size_t  n;    //!< exponent
            prime_factor *make_one() throw();
            
            //__________________________________________________________________
            //
            // serializable
            //__________________________________________________________________
            virtual const char   *className() const throw();                    //!< CLASS_NAME
            virtual size_t        serialize(ios::ostream &) const;              //!< [n+p]
            static  prime_factor  read(ios::istream &, size_t &, const char *); //!< relaod

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            natural value() const; //!< explicit computation
            friend bool operator==(const prime_factor &lhs, const prime_factor &rhs) throw(); //!< ==
            friend bool operator!=(const prime_factor &lhs, const prime_factor &rhs) throw(); //!< !=
            //! display
            friend std::ostream & operator<<( std::ostream &os, const prime_factor &f);

        private:
            void setup() throw();
        };

    }
}

#endif
