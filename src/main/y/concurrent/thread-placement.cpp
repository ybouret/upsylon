#include "y/concurrent/thread.hpp"
#include "y/exceptions.hpp"
#include <iostream>

#if defined(Y_THREAD_AFFINITY)
#     undef Y_THREAD_AFFINITY
#endif

namespace upsylon
{
    namespace concurrent
    {

        namespace nucleus
        {

#if defined(Y_LINUX)|| defined(Y_FREEBSD)
#   define Y_THREAD_AFFINITY 1
#   if defined(Y_FREEBSD)
#      include <pthread_np.h>
#      define Y_CPU_SET cpuset_t
#   else
#      define Y_CPU_SET cpu_set_t
#endif

            void   thread:: assign(handle h,const size_t j)
            {
                Y_CPU_SET the_cpu_set;
                CPU_ZERO(  &the_cpu_set );
                CPU_SET(j, &the_cpu_set );
                const int err = pthread_setaffinity_np( h, sizeof(Y_CPU_SET), &the_cpu_set );
                if( err != 0 )
                    throw libc::exception( err, "pthread_setaffinity_np" );
            }
#endif

#if defined(Y_WIN)
#define Y_THREAD_AFFINITY 1
            void thread:: assign( handle h, const size_t j )
            {
                const DWORD_PTR mask = DWORD_PTR(1) << j;
                if( ! ::SetThreadAffinityMask( h, mask ) )
                {
                    const DWORD err = ::GetLastError();
                    throw win32::exception( err, "::SetThreadAffinityMask" );
                }
            }
#endif

#if 0 
#   define Y_THREAD_AFFINITY 1
#   include <pthread_np.h>
#   include <sched.h>
            extern "C"
            {
                struct cpuset * cpuset_create();
                void cpuset_zero(struct cpuset *);
                void cpuset_set(int,struct cpuset *);
                void cpuset_destroy(struct cpuset*);
                size_t cpuset_size(struct cpuset*);
            }
            void thread:: assign( handle h, const size_t j )
            {
                std::cerr << "\t\t" Y_PLATFORM "@" << j << std::endl;
                struct cpuset *info = cpuset_create();
                if(!info)
                {
                    throw libc::exception(ENOMEM,"cpuset_create");
                }
                cpuset_zero(info);
                cpuset_set(j,info);
                const int err = pthread_setaffinity_np( h, cpuset_size(info), info );
                cpuset_destroy(info);
                if( err != 0 )
                    throw libc::exception( err, "pthread_setaffinity_np" );
            }
#endif

#if defined(Y_APPLE)
#define Y_THREAD_AFFINITY 1
#include <mach/thread_policy.h>
#include <mach/thread_act.h>
            void thread::assign(handle h, const size_t j)
            {
                thread_affinity_policy_data_t policy_data = { j };
                mach_port_t                   mach_thread = pthread_mach_thread_np(h);
                if( KERN_SUCCESS != thread_policy_set(mach_thread, THREAD_AFFINITY_POLICY, (thread_policy_t)&policy_data, THREAD_AFFINITY_POLICY_COUNT))
                {
                    throw exception("thread_policy_set failure");
                }
            }
#endif

#if defined(Y_SUNOS)
#include <sys/types.h>
#include <sys/processor.h>
#include <sys/procset.h>
#define Y_THREAD_AFFINITY 1
            void thread::assign(handle h, const size_t j)
            {
                const int res = processor_bind(P_LWPID,idtype_t(h),j,NULL);
                if(0!=res)
                    throw exception("processor_bind failure");
            }

#endif

#if !defined(Y_THREAD_AFFINITY)
            // fallback
            void   thread:: assign(handle,const size_t)
            {
            }
#endif

        }

    }

}

