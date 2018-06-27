#include "y/os/pid.hpp"

namespace upsylon
{
    process_id::type process_id::get()
    {
#if defined(Y_BSD)
        return getpid();
#endif

#if defined(Y_WIN)
        return ::GetCurrentProcessId();
#endif

    }

    uint32_t process_id::h32()
    {
        const type p = get();
        return crc32(&p, sizeof(p));
    }
}

