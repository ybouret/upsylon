//! \file

#ifndef Y_CONCURRENT_TASK_CONTRACTS_INCLUDED
#define Y_CONCURRENT_TASK_CONTRACTS_INCLUDED 1

#include "y/concurrent/task/contract.hpp"

namespace upsylon
{
    namespace concurrent
    {
        
        class contracts : public core::list_of<contract>, public releasable
        {
        public:
            virtual ~contracts() throw();
            
        protected:
            explicit contracts() throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(contracts);
            
        };

#if 0
        class pending_contracts : public contracts
        {
        public:
            explicit     pending_deeds() throw();
            virtual     ~pending_deeds() throw();
            virtual void release()       throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(pending_deeds);
        };
        
        class shallow_deeds : public contracts
        {
        public:
            explicit shallow_deeds() throw();
            virtual ~shallow_deeds() throw();
            virtual void release() throw();
            void         reserve(size_t n);
            void         cancel(deed *)          throw();
            void         cancel(pending_deeds &) throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(shallow_deeds);
        };
#endif
        
     
        
       
        
    }
    
}

#endif
