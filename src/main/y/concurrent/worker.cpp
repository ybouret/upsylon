

#include "y/concurrent/worker.hpp"
#include "y/type/block/zset.hpp"
#include "y/exceptions.hpp"
#include "y/os/error.hpp"

namespace upsylon {

    namespace concurrent
    {

        namespace nucleus
        {
            static inline Y_THREAD_LAUNCHER_RETURN thread_launcher( Y_THREAD_LAUNCHER_PARAMS args ) throw()
            {
                assert(args);
                concurrent::worker &w = *static_cast<concurrent::worker*>(args);
                assert(w.proc);
                w.proc(w.data);
                return 0;
            }

            thread::handle thread:: launch_worker( void *args, ID &tid)
            {
                assert(args);
                bzset(tid);
#if    defined(Y_BSD)
                const int res = pthread_create( &tid, NULL, thread_launcher,args);
                if( res != 0 )
                {
                    throw libc::exception( res, "pthread_create" );
                }
                return tid;
#endif

#if defined(Y_WIN)
                Y_GIANT_LOCK();
                handle h = ::CreateThread(0 ,
                                          0 ,
                                          thread_launcher,
                                          args,
                                          0,
                                          &tid );
                if( NULL == h )
                {
                    const DWORD res = ::GetLastError();
                    throw win32::exception( res, "::CreateThread" );
                }
                return h;
#endif
            }

        }



        worker:: worker(call         user_proc,
                        void        *user_data,
                        const size_t user_size,
                        const size_t user_rank) :
        context(user_size,user_rank),
        proc(user_proc),
        data(user_data),
        id(),
        handle( nucleus::thread::launch_worker(this,(nucleus::thread::ID &)id) )
        {
        }

        worker:: ~worker() throw()
        {
            nucleus::thread::finish((nucleus::thread::handle&)handle);
            bzset( (nucleus::thread::ID &)id );
        }

        
    }

}

