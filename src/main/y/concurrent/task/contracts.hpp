
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
            
            void hire(size_t n);               //!< reserve zombies
            void keep(const size_t n) throw(); //!< zombies.size<=n
            void trim() throw();               //!< keep(0)
            
            core::list_of<contract> pending; //!< pending, alive
            core::list_of<contract> zombies; //!< memory, dangling
            
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(contracts);
        };

    }
    
}

#endif

