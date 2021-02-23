
//! \file

#ifndef Y_CONCURRENT_TASK_CONTRACT_INCLUDED
#define Y_CONCURRENT_TASK_CONTRACT_INCLUDED 1

#include "y/concurrent/task/types.hpp"

namespace upsylon
{
    namespace concurrent
    {

        class contract
        {
        public:

            contract       *next;
            contract       *prev;
            const job::uuid uuid;
            const job::type type;

            contract(const job::uuid U, const job::type &J);
            ~contract() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(contract);
        };

        class contracts
        {
        public:
            core::list_of<contract> pending;
            core::list_of<contract> zombies;

            explicit contracts() throw();
            virtual ~contracts() throw();

            void establish(const job::uuid U, const job::type &J);
            void terminate(contract *c) throw();

            void gc() throw();

            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(contracts);
        };


    }
}


#endif
