
//! \file
#ifndef Y_AP_PRIME_INCLUDED
#define Y_AP_PRIME_INCLUDED 1

#include "y/yap/natural.hpp"
#include "y/core/list.hpp"

namespace upsylon
{

    namespace yap
    {

        //______________________________________________________________________
        //
        //
        //! prime  >= 5
        //
        //______________________________________________________________________
        class prime : public natural
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            typedef core::list_of_cpp<prime> list_type; //!< to hold the primes

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit prime(const natural &); //!< initialize
            explicit prime(const utype    ); //!< initialize
            virtual ~prime() throw();        //!< cleanup

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            prime *       next;    //!< for list
            prime *       prev;    //!< for list
            const natural squared; //!< (*this)^2
            const natural add_two; //!< (this)+2


        private:
            Y_DISABLE_COPY_AND_ASSIGN(prime);
        };

    }

}

#endif
