
//! \file

#ifndef Y_AP_PRIME_FACTORS_INCLUDED
#define Y_AP_PRIME_FACTORS_INCLUDED 1

#include "y/yap/prime/factor.hpp"
#include "y/core/roll.hpp"

namespace upsylon {

    namespace yap {

        //______________________________________________________________________
        //
        // common definition
        //______________________________________________________________________
        typedef core::roll_of_cpp<prime_factor> pf_list; //!< alias

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
            static const char CLASS_NAME[];                       //!< "yapF";

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            prime_factors() throw();                           //!< zero
            prime_factors(const natural &n);                   //!< decompose
            prime_factors(const utype    u);                   //!< decompose
            prime_factors(const prime_factors &);              //!< copy
            prime_factors & operator=( const prime_factors &); //!< assign
            virtual ~prime_factors() throw();                  //!< cleanup

            //! display
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
            natural value() const;                //!< compute value
            void    xch(prime_factors &) throw(); //!< no-throw
            bool    is_zero() const throw();      //!< check is zero
            bool    is_one()  const throw();      //!< check is one

            //__________________________________________________________________
            //
            // multiplication
            //__________________________________________________________________
            static  prime_factors mul(const prime_factors &lhs, const prime_factors &rhs); //!< multiplication
            static  prime_factors mul(const prime_factors &lhs, const natural       &rhs); //!< multipication wrapper
            static  prime_factors mul(const prime_factors &lhs, const utype          rhs); //!< multiplication wrapper

            inline friend prime_factors operator*(const prime_factors &lhs, const prime_factors &rhs) { return mul(lhs,rhs); } //!< multiplication
            inline friend prime_factors operator*(const prime_factors &lhs, const natural       &rhs) { return mul(lhs,rhs); } //!< multiplication
            inline friend prime_factors operator*(const prime_factors &lhs, const utype          rhs) { return mul(lhs,rhs); } //!< multiplication
            inline friend prime_factors operator*(const natural       &lhs, const prime_factors &rhs) { return mul(rhs,lhs); } //!< multiplication
            inline friend prime_factors operator*(const utype          lhs, const prime_factors &rhs) { return mul(rhs,lhs); } //!< multiplication

            inline prime_factors & operator*=(const prime_factors &rhs) { prime_factors tmp = mul(*this,rhs); xch(tmp); return *this; } //!< in-place multiplication
            inline prime_factors & operator*=(const natural       &rhs) { prime_factors tmp = mul(*this,rhs); xch(tmp); return *this; } //!< in-place multiplication
            inline prime_factors & operator*=(const utype          rhs) { prime_factors tmp = mul(*this,rhs); xch(tmp); return *this; } //!< in-place multiplication


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const pf_list factors; //!< list of factors, increasing

        private:
            void make(const natural &n);

        };

    }

}

#endif
