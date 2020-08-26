
//! \file

#ifndef Y_AP_SPRP_INCLUDED
#define Y_AP_SPRP_INCLUDED 1

#include "y/yap/natural.hpp"

namespace upsylon
{

    namespace yap
    {

        //______________________________________________________________________
        //
        //
        //! Strong PRobable Prime architecture
        //
        //______________________________________________________________________
        class sprp
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit sprp(const natural     &); //!< build for odd value
            explicit sprp(const number::utype); //!< build for odd value
            sprp(const sprp &);                 //!< copy
            virtual ~sprp() throw();            //!< cleanup

            //__________________________________________________________________
            //
            // check functions
            //__________________________________________________________________
            bool base(const natural      &a) const; //!< check a-SPRP
            bool base(const number::utype a) const; //!< check a-SPRP

            const natural n; //!< should be an odd number
            const natural m; //!< n-1 is even
            const natural d; //!< odd part
            const size_t  s; //!< exponent part, m=2^s*d

            //__________________________________________________________________
            //
            // helper
            //__________________________________________________________________
            static natural end(const natural      &n); //!< compute Miller's Test 2(log(n)^2)
            static natural end(const number::utype n); //!< compute Miller's Test 2(log(n)^2)

        private:
            Y_DISABLE_ASSIGN(sprp);
            void check() const;
            void make() throw();
        };

        

    }
}

#endif
