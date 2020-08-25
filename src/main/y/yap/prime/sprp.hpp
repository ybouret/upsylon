
//! \file

#ifndef Y_AP_SPRP_INCLUDED
#define Y_AP_SPRP_INCLUDED 1

#include "y/yap/natural.hpp"

namespace upsylon
{

    namespace yap
    {

        class sprp
        {
        public:
            explicit sprp(const natural &);
            explicit sprp(const number::utype);
            sprp(const sprp &);
            virtual ~sprp() throw();

            const natural n; //!< should be an odd number
            const natural m; //!< n-1 is even
            const natural d; //!< odd part
            const size_t  s; //!< exponent part

            bool is(const natural     &) const;
            bool is(const number::utype) const;

        private:
            Y_DISABLE_ASSIGN(sprp);
            void make() throw();
        };

    }
}

#endif
