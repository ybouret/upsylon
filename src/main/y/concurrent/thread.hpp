
//! \file

#ifndef Y_CONCURRENT_THREAD_INCLUDED
#define Y_CONCURRENT_THREAD_INCLUDED 1

#include "y/concurrent/context.hpp"
#include "y/concurrent/nucleus/thread.hpp"

namespace upsylon
{

    namespace concurrent
    {
        //______________________________________________________________________
        //
        //
        //! a thread is a context with a threaded function
        //
        //______________________________________________________________________
        class thread : public context
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit thread(nucleus::thread::call user_call,
                            void                 *user_args,
                            const size_t          user_size,
                            const size_t          user_rank); //!< setup
            virtual ~thread() throw();                        //!< cleanup
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            nucleus::thread::call         proc;   //!< procedure to call
            void                         *args;   //!< PERSISTENT argument
            const nucleus::thread::ID     id;     //!< system ID
            const nucleus::thread::handle handle; //!< system handle

        private:
            Y_DISABLE_COPY_AND_ASSIGN(thread);
            
        };
        
      
    }
}

#endif

