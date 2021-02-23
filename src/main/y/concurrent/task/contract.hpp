
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

        //______________________________________________________________________
        //
        //
        //! manage contracts with memoryc caching
        //
        //______________________________________________________________________
        class contracts
        {
        public:

            explicit contracts() throw();
            virtual ~contracts() throw();

            void establish(const job::uuid U, const job::type &J); //!< add to pending
            void terminate(contract *c) throw();                   //!< destruct and store to zombies

            void gc() throw(); //!< remove zombies

            core::list_of<contract> pending; //!< pending, alive
            core::list_of<contract> zombies; //!< memory, dangling

            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(contracts);
        };


    }
}


#endif
