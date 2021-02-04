#include "y/memory/locked.hpp"
#include "y/os/page-size.hpp"
#include "y/concurrent/mutex.hpp"
#include "y/exceptions.hpp"
#include <cstring>

#if defined(Y_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#if defined(Y_BSD)
#include <sys/mman.h>
#include <cerrno>
#endif

namespace upsylon
{
    namespace memory
    {
        locked_area:: ~locked_area() throw()
        {
        }

        static inline size_t locked_bytes(size_t desiredBytes)
        {
            const size_t ps = page_size::get();
            const size_t q  = desiredBytes/ps;
            const size_t r  = desiredBytes%ps;
            size_t       res  = q * ps;
            if(r)        res += ps;
            return res;
        }


        static inline void *acquire_locked(const size_t bs)
        {
            Y_GIANT_LOCK();
#if defined(Y_WIN)
            void *addr = VirtualAlloc(NULL,bs, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
            if (!addr)
            {
                throw win32::exception(GetLastError(), "VirtualAlloc(%lu)",(unsigned long)bs);
            }


            if(!VirtualLock(addr,bs))
            {
                VirtualFree(addr,0,MEM_RELEASE);
                throw win32::exception(GetLastError(), "VirtualLock(%p,%lu)",addr,(unsigned long)bs);
            }
#endif

#if defined(Y_BSD)
            void *addr = malloc(bs);
            if(!addr) throw libc::exception(errno,"acquire_locked(%lu)", (unsigned long)bs);
            if(mlock(addr,bs)!=0)
            {
                free(addr);
                throw libc::exception(errno,"mlock(%p,%lu)",addr,(unsigned long)bs);
            }
#endif
            memset(addr,0,bs);
            return addr;

        }


        locked_area:: locked_area(const size_t desiredBytes) :
        bytes( locked_bytes(desiredBytes) ),
        entry( acquire_locked(bytes)      )
        {

        }

    }
}

