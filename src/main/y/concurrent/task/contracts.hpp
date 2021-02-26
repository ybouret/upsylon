//! \file

#ifndef Y_CONCURRENT_TASK_CONTRACTS_INCLUDED
#define Y_CONCURRENT_TASK_CONTRACTS_INCLUDED 1

#include "y/concurrent/task/contract.hpp"

namespace upsylon
{
    namespace concurrent
    {

        //! list of contracts
        class contracts : public core::list_of<contract>, public releasable
        {
        public:
            virtual ~contracts() throw();
            
        protected:
            explicit contracts() throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(contracts);
            
        };


        class settled; //!< forward declaration

        class pending : public contracts
        {
        public:
            explicit     pending() throw();
            virtual     ~pending() throw();
            virtual void release() throw();

            void         append(const job::uuid, const job::type &, settled &pool);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(pending);
        };

        
        class settled : public contracts
        {
        public:
            explicit     settled() throw();
            virtual     ~settled() throw();
            virtual void release() throw();
            void         reserve(size_t n);
            void         cancel(contract *) throw();
            void         cancel(pending  &) throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(settled);
        };

     
        
       
        
    }
    
}

#endif
