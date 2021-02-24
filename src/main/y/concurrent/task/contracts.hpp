
//! \file

#ifndef Y_CONCURRENT_TASK_CONTRACTS_INCLUDED
#define Y_CONCURRENT_TASK_CONTRACTS_INCLUDED 1

#include "y/concurrent/task/contract.hpp"

namespace upsylon
{
    namespace concurrent
    {
        //______________________________________________________________________
        //
        //
        //! manage contracts with memory caching
        //
        //______________________________________________________________________
        class contracts
        {
        public:
            
            explicit contracts() throw();
            virtual ~contracts() throw();
            
            void establish(const job::uuid U, const job::type &J); //!< add to pending
            void terminate(contract * &c) throw();                   //!< destruct and store to shallow

            void reserve(size_t n);
            void prune() throw();

            static void delete_shallow(contract *z) throw();
            static void delete_pending(contract *c) throw();
            void        remove_shallow() throw();
            void        remove_pending() throw();
            void        release()        throw();

            core::list_of<contract> pending; //!< pending, alive
            core::list_of<contract> shallow; //!< memory, dangling
            
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(contracts);
        };

    }
    
}

#endif

