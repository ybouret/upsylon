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
        //! holds a user's job and its ID
        //
        //______________________________________________________________________
        class contract
        {
        public:
            contract(const job::uuid, const job::type &); //!< setup
            ~contract() throw();                          //!< cleanup
            
            static contract *zcreate();                   //!< create zombie
            static contract *revoked(contract *) throw(); //!< alive -> zombie
            static void      zdelete(contract *) throw(); //!< delete zombie
            static void      release(contract *) throw(); //!< delete alive
            
            contract       *next; //!< for list
            contract       *prev; //!< for list
            const job::uuid uuid; //!< uuid
            const job::type plan; //!< what to do
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(contract);
        };
        
    }
    
}

#endif
