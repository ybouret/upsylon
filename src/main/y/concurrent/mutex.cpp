#include "y/concurrent/mutex.hpp"
#include "y/os/error.hpp"
#include "y/type/utils.hpp"

#include <cstring>
#include <iostream>



namespace upsylon
{
    namespace concurrent
    {

        namespace nucleus
        {

            void mutex:: init(type *m) throw()
            {
                //______________________________________________________________
                //
                // prepare
                //______________________________________________________________
                assert(m!=0);
                memset(m,0,sizeof(type));

                //______________________________________________________________
                //
                // setup
                //______________________________________________________________
#if defined(Y_WIN)
                ::InitializeCriticalSection(m);
#endif
                
#if defined(Y_BSD)
                    pthread_mutexattr_t attr;

                    int  res = pthread_mutexattr_init( &attr );
                    if (res != 0)
                        libc::critical_error(res, "pthread_mutexattr_init");

                    res = pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_RECURSIVE);
                    if (res != 0)
                    {
                        pthread_mutexattr_destroy( &attr );
                        libc::critical_error(res, "pthread_mutexattr_settype(RECURSIVE)");
                    }

                    res = pthread_mutex_init(m,&attr);
                    (void) pthread_mutexattr_destroy( &attr );
                    if( res != 0 )
                        libc::critical_error(res,"pthread_mutex_init");
#endif

            }

            void mutex:: quit(type *m) throw()
            {
                assert(m);
                //std::cerr << "quit mutex" << std::endl;
#if defined(Y_WIN)
                ::DeleteCriticalSection( m );
#endif

#if defined(Y_BSD)
                {
                    const int res = pthread_mutex_destroy( m );
                    if( res != 0 )
                        libc::critical_error( res, "pthread_mutex_destroy" );
                }
#endif
                memset(m,0,sizeof(type));
            }

            void mutex:: lock(type *m) throw()
            {
                assert(m);
#if defined(Y_WIN)
                ::EnterCriticalSection(m);
#endif

#if defined(Y_BSD)
                const int res = pthread_mutex_lock(m);
                if( res != 0 )
                    libc::critical_error( res,  "pthread_mutex_lock" );
#endif
            }

            void mutex:: unlock(type *m) throw()
            {
                assert(m);
#if defined(Y_WIN)
                ::LeaveCriticalSection(m);
#endif

#if defined(Y_BSD)
                const int res = pthread_mutex_unlock(m);
                if( res != 0 )
                    libc::critical_error( res,  "pthread_mutex_lock" );
#endif
            }

            bool mutex:: try_lock(type *m) throw()
            {
                assert(m);
#if defined(Y_WIN)
                return TryEnterCriticalSection(m) == TRUE;
#endif

#if defined(Y_BSD)
                return pthread_mutex_trylock(m) == 0;
#endif
            }
        }

        mutex mutex::giant;
        void mutex:: __format(const char *id) throw()
        {
            char *target = (char *)name;
            memset(target,0,sizeof(name));

            if(id)
            {
                memcpy( target,id, min_of<size_t>(strlen(id),mutex_name_size-1) );
            }
            else
            {
                const void *self = this;
                const char *addr = (const char *)self;
                ptrdiff_t   p    = static_cast<ptrdiff_t>(addr-(char*)0);
                
            }
        }

    }

}

