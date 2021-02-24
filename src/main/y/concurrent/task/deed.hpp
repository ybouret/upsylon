//! \file

#ifndef Y_CONCURRENT_TASK_DEED_INCLUDED
#define Y_CONCURRENT_TASK_DEED_INCLUDED 1

#include "y/concurrent/task/types.hpp"

namespace upsylon
{
    namespace concurrent
    {
        
        class deed
        {
        public:
            deed           *next;
            deed           *prev;
            const job::uuid uuid;
            const job::type func;
            
            deed(const job::uuid, const job::type &);
            ~deed() throw();
        
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(deed);
        };
        
    }
    
}

#endif
