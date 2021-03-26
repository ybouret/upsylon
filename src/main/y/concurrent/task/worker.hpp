//! \file

#ifndef Y_CONCURRENT_TASK_WORKER_INCLUDED
#define Y_CONCURRENT_TASK_WORKER_INCLUDED 1


#include "y/concurrent/task/contract.hpp"
#include "y/concurrent/thread.hpp"
#include "y/concurrent/sync/condition.hpp"
#include "y/core/inode.hpp"
#include "y/type/authority.hpp"

namespace upsylon
{
    namespace concurrent
    {

        class pipeline; //!< forward declaration
               
        //______________________________________________________________________
        //
        //
        //! distinguishable thread
        //
        //______________________________________________________________________
        class worker :
        public authority<pipeline>,
        public condition,
        public thread,
        public inode<worker>
        {
        public:
            //! alias
            typedef core::list_of_cpp<worker> list_type; 
            
            //! setup
            explicit worker(pipeline    &user_pipe,
                            const size_t user_size,
                            const size_t user_rank);
            //! cleanup
            virtual ~worker() throw();
            
            //! (mutable) deal to handle
            contract *deal;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(worker);
            static void stub(void*) throw();
        };
    }
}

#endif
