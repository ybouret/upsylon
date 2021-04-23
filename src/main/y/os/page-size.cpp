
#include "y/os/page-size.hpp"
#include "y/exceptions.hpp"
#include "y/concurrent/sync/mutex.hpp"

#if defined(Y_BSD)
#include <unistd.h>
#include <cerrno>
#endif

#if defined(Y_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "y/type/block/zset.hpp"
#endif

namespace upsylon
{
    size_t page_size:: get()
    {
        Y_GIANT_LOCK();
#if defined(Y_BSD)
        const int res = getpagesize();
        if(res<=0) throw libc::exception(EINVAL,"getpagesize=%d",res);
        return size_t(res);
#endif

#if defined(Y_WIN)
        SYSTEM_INFO sSysInfo;
        bzset(sSysInfo);
        GetSystemInfo(&sSysInfo);
        const uint32_t res = sSysInfo.dwPageSize;
        if(res<=0) throw win32::exception(ERROR_INVALID_DATA,"GetSystemInfo(PageSize)");
        return size_t(res);
#endif

    }
}
