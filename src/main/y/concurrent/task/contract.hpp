//! \file

#ifndef Y_CONCURRENT_TASK_CONTRACT_INCLUDED
#define Y_CONCURRENT_TASK_CONTRACT_INCLUDED 1

#include "y/concurrent/task/types.hpp"

namespace upsylon
{
    namespace concurrent
    {
        //______________________________________________________________________
        //
        //
        //! a contract is a dynamic job
        //
        //______________________________________________________________________
        class contract
        {
        public:
            contract       *next; //!< for list
            contract       *prev; //!< for list
            const job::uuid uuid; //!< UUID for job
            const job::type type; //!< job content

            contract(const job::uuid U, const job::type &J); //!< setup
            ~contract() throw();                             //!< cleanup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(contract);
        };

       

    }
}


#endif
