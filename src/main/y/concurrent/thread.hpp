
//! \file

#ifndef Y_CONCURRENT_THREAD_INCLUDED
#define Y_CONCURRENT_THREAD_INCLUDED 1

#include "y/concurrent/context.hpp"
#include "y/concurrent/nucleus/thread.hpp"

namespace upsylon
{

    namespace concurrent
    {
        

        //!
        class thread : public context
        {
        public:
            explicit thread(nucleus::thread::call user_call,
                            void                 *user_args,
                            const size_t          user_size,
                            const size_t          user_rank);

            virtual ~thread() throw();
            

            nucleus::thread::call         proc;
            void                         *args;
            const nucleus::thread::ID     id;
            const nucleus::thread::handle handle;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(thread);
            
        };
        
        //! T.run(context)
        template <typename T>
        class runnable : public thread
        {
        public:
            explicit runnable(T           &user_host,
                              const size_t user_size,
                              const size_t user_rank) throw() :
            thread(call_host,
                   this,
                   user_size,
                   user_rank),
            host(user_host)
            {
            }
            
            inline virtual ~runnable() throw()
            {
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(runnable);
            T &host;
            static inline void call_host(void *addr)
            {
                assert(addr);
                runnable &self = *static_cast<runnable *>(addr);
                self.host.run(self);
            }
        };

    }
}

#endif

