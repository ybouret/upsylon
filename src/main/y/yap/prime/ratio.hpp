

//! \file

#ifndef Y_AP_PRIME_RATIO_INCLUDED
#define Y_AP_PRIME_RATIO_INCLUDED 1

#include "y/yap/prime/factors.hpp"

namespace upsylon {

    namespace yap {

        //______________________________________________________________________
        //
        //
        //! a prime factors decomposition
        //
        //______________________________________________________________________
        class prime_ratio : public number
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const char CLASS_NAME[];                       //!< "yapR";

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit prime_ratio();
            prime_ratio(const prime_ratio &);
            prime_ratio & operator=(const prime_ratio &);
            virtual ~prime_ratio() throw();

#define Y_APPQ_CTOR(LHS,RHS) \
inline prime_ratio(const LHS lhs, const RHS rhs) : num(lhs), den(rhs) { check(); update(); }

            Y_APPQ_CTOR(natural&,natural&)
            Y_APPQ_CTOR(utype   ,natural&)
            Y_APPQ_CTOR(natural &,utype  )
            Y_APPQ_CTOR(utype    ,utype  )


            //__________________________________________________________________
            //
            // serializable
            //__________________________________________________________________
            virtual const char * className() const throw();
            virtual size_t       serialize(ios::ostream &) const;

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            friend std::ostream & operator<<(std::ostream &, const prime_ratio &);
            void xch( prime_ratio &) throw();
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const prime_factors num;
            const prime_factors den;

        private:
            void check()  const;
            void update() throw();
        };
    }
}
#endif

