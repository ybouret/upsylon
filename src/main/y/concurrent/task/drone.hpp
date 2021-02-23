//! \file

#ifndef Y_CONCURRENT_TASK_DRONE_INCLUDED
#define Y_CONCURRENT_TASK_DRONE_INCLUDED 1

#include "y/concurrent/task/contract.hpp"
#include "y/concurrent/executable.hpp"
#include "y/concurrent/sync/condition.hpp"
#include "y/core/inode.hpp"
#include "y/type/authority.hpp"

namespace upsylon
{
    namespace concurrent
    {

        class pipeline;

        class drone:
        public object,
        public condition,
        public authority<pipeline>,
        public thread,
        public inode<drone>
        {
        public:
            explicit drone(pipeline     &user_pipe,
                           const size_t user_size,
                           const size_t user_rank);

            virtual ~drone() throw();


        private:
            Y_DISABLE_COPY_AND_ASSIGN(drone);
            static void stub(void*) throw();    //!< call threaded loop
            void        loop()      throw();   //!< initialize function: wait...
        };


    }

}

#endif
