
//! \file

#ifndef Y_CONCURRENT_TASK_SERIAL_INCLUDED
#define Y_CONCURRENT_TASK_SERIAL_INCLUDED 1

#include "y/concurrent/task/type.hpp"
#include "y/concurrent/sync/fake-lock.hpp"


namespace upsylon
{
    namespace concurrent
    {


        class serial
        {
        public:
            virtual ~serial() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(serial);
        };

    }

}


#endif
