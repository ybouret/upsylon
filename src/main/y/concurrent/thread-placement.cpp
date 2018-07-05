
#include "y/concurrent/thread.hpp"
#include "y/exceptions.hpp"
#include <iostream>

#if defined(Y_WIN)
#       define Y_THREAD_AFFINITY 1
#endif

#if defined(Y_LINUX) || defined(Y_OPENBSD) || defined(Y_SUNOS)
#       define Y_CPU_SET_PTHREAD 1
#       define Y_CPU_SET         cpu_set_t
#       define Y_THREAD_AFFINITY 1
#endif

#if defined(Y_FREEBSD)
#       include <pthread_np.h>
#       define Y_CPU_SET_PTHREAD 1
#       define Y_CPU_SET         cpuset_t
#       define Y_THREAD_AFFINITY 1
#endif

#if defined(Y_APPLE)
/*
#define Y_THREAD_AFFINITY 1
#include <mach/thread_policy.h>
kern_return_t    thread_policy_set(thread_t                    thread,
                                   thread_policy_flavor_t      flavor,
                                   thread_policy_t             policy_info,
                                   mach_msg_type_number_t      count);
 */
#endif

namespace upsylon
{

    namespace concurrent
    {
        namespace nucleus
        {
#if defined(Y_WIN)
            static inline void __assign( thread::handle &h, const size_t j )
            {
                std::cerr << "\t\t" << Y_PLATFORM << "@" << j << std::endl;
                const DWORD_PTR mask = DWORD_PTR(1) << j;
                if( ! ::SetThreadAffinityMask( h, mask ) )
                {
                    const DWORD err = ::GetLastError();
                    throw win32::exception( err, "::SetThreadAffinityMask" );
                }
            }
#endif

#if defined(Y_CPU_SET_PTHREAD)
            static  void __assign(  thread::handle &h, const size_t j )
            {
                std::cerr << "\t\t" << Y_PLATFORM << "@" << j << std::endl;
                Y_CPU_SET cpu_set;
                CPU_ZERO(  &cpu_set );
                CPU_SET(j, &cpu_set );
                const int err = pthread_setaffinity_np( h, sizeof(Y_CPU_SET), &cpu_set );
                if( err != 0 )
                    throw libc::exception( err, "pthread_setaffinity_np" );
            }
#endif

            /*
#if defined(Y_APPLE)
            static void __assign( thread::handle &h, const size_t j)
            {
                int core = int(j);
                thread_affinity_policy_data_t policy = { core };
                thread_port_t mach_thread = pthread_mach_thread_np(h);
                thread_policy_set(mach_thread, THREAD_AFFINITY_POLICY,
                                  (thread_policy_t)&policy, 1);
            }

#endif
             */

#if !defined(Y_THREAD_AFFINITY)
            static void __assign( thread::handle &, const size_t ) throw()
            {
            }
#endif

            void   thread:: assign( handle h, const size_t cpu )
            {
                __assign(h,cpu);
            }


        }
    }

}

