
//! \file
#ifndef Y_AP_PRIME_INCLUDED
#define Y_AP_PRIME_INCLUDED 1

#include "y/yap/natural.hpp"
#include "y/core/list.hpp"

namespace upsylon
{

    namespace yap
    {

        //! prime  >= 5
        class prime : public natural
        {
        public:
            typedef core::list_of_cpp<prime> list_type;

            explicit prime(const natural &);
            explicit prime(const utype    );
            virtual ~prime() throw();

            prime *       next;
            prime *       prev;
            const natural squared; //!< (*this)^2
            const natural add_two; //!< (this)+2


        private:
            Y_DISABLE_COPY_AND_ASSIGN(prime);
        };

    }

}

#endif
