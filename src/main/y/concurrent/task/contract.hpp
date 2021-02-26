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
            const job::type plan;
            
            contract(const job::uuid, const job::type &);
            ~contract() throw();
        
            static contract *zcreate();
            static contract *revoked(contract *) throw();
            static void      zdelete(contract *) throw();
            static void      release(contract *) throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(contract);
        };
        
    }
    
}

#endif