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
        //! list of contracts
        //
        //______________________________________________________________________
        class contracts : public core::list_of<contract>, public releasable
        {
        public:
            virtual ~contracts() throw(); //!< cleanup, call release()
            
        protected:
            explicit contracts() throw(); //!< setup
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(contracts);
            
        };


        class settled; //!< forward declaration
        
        //______________________________________________________________________
        //
        //
        //! pedning/active contracts
        //
        //______________________________________________________________________
        class pending : public contracts
        {
        public:
            explicit     pending() throw(); //!< setup
            virtual     ~pending() throw(); //!< cleanup
            virtual void release() throw(); //!< release all memory

            //! create a new contract, using settled as pool
            void         append(const job::uuid, const job::type &, settled &pool);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(pending);
        };

        //______________________________________________________________________
        //
        //
        //! settled/zombie contracts
        //
        //______________________________________________________________________
        class settled : public contracts
        {
        public:
            explicit     settled() throw(); //!< setup
            virtual     ~settled() throw(); //!< cleanup
            virtual void release() throw(); //!< release all memory
            void         reserve(size_t n); //!< reserve some memory
            void         cancel(contract *) throw(); //!< cancel/store a contract
            void         cancel(pending  &) throw(); //!< cancel/store some contracts
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(settled);
        };

     
        
       
        
    }
    
}

#endif
