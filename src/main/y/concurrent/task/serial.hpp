
//! \file

#ifndef Y_CONCURRENT_TASK_SERIAL_INCLUDED
#define Y_CONCURRENT_TASK_SERIAL_INCLUDED 1

#include "y/concurrent/task/types.hpp"
#include "y/concurrent/sync/fake-lock.hpp"


namespace upsylon
{
    namespace concurrent
    {

        //______________________________________________________________________
        //
        //
        //! serial tasks
        //
        //______________________________________________________________________
        class serial : public supervisor
        {
        public:
            virtual ~serial() throw(); //!< cleanup
            explicit serial() throw(); //!< setup

            virtual job::uuid enqueue( const job::type & );

        private:
            Y_DISABLE_COPY_AND_ASSIGN(serial);
            fake_lock access;
        };

    }

}


#endif
