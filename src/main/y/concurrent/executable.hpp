
//! \file

#ifndef Y_CONCURRENT_EXECUTABLE_INCLUDED
#define Y_CONCURRENT_EXECUTABLE_INCLUDED 1

#include "y/concurrent/context.hpp"
#include "y/concurrent/sync/mutex.hpp"



namespace upsylon
{

    namespace concurrent
    {

        //! function to execute on a context (i.e. within a thread)
        class executable
        {
        public:
            virtual ~executable() throw();
            virtual void call(const context &) = 0;

            mutex access;
            

        protected:
            explicit executable() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(executable);
        };

    }

}

#endif

