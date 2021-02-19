
//! \file

#ifndef Y_CONCURRENT_TASK_PIPELINE_INCLUDED
#define Y_CONCURRENT_TASK_PIPELINE_INCLUDED 1

#include "y/concurrent/task/type.hpp"
#include "y/concurrent/executable.hpp"
#include "y/concurrent/thread.hpp"
#include "y/core/inode.hpp"

namespace upsylon
{
    namespace concurrent
    {

        class pipeline : public executable
        {
        public:
            class engine : public object, public thread, public inode<engine>
            {

            };

            virtual ~pipeline() throw();
            explicit pipeline() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(pipeline);
            virtual void call(const context &) throw();
            
        };
    }
}


#endif

