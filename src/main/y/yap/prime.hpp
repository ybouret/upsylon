
//! \file
#ifndef Y_AP_PRIME_INCLUDED
#define Y_AP_PRIME_INCLUDED 1

#include "y/yap/natural.hpp"
#include "y/core/inode.hpp"
#include "y/core/list.hpp"

namespace upsylon
{

    namespace yap
    {

        class prime : public natural, public inode<prime>
        {
        public:
            typedef core::list_of_cpp<prime> list_type;

            explicit prime(const natural &);
            explicit prime(const utype    );
            virtual ~prime() throw();

            const natural q; //!< (*this)^2


        private:
            Y_DISABLE_COPY_AND_ASSIGN(prime);
        };

    }

}

#endif
