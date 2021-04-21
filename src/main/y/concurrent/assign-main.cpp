
#include "y/concurrent/assign-main.hpp"
#include "y/os/hw.hpp"
#include "y/os/pid.hpp"
#include "y/concurrent/nucleus/thread.hpp"

namespace upsylon
{
    size_t assign_main:: max_cpus()
    {
        return hardware::nprocs();
    }

    void assign_main::to(const size_t cpu, const char *who)
    {
        concurrent::nucleus::thread::assign_current(cpu,who);
    }

    size_t assign_main::by_pid(const char *who)
    {
        const size_t n = max_cpus();
        if(n>1)
        {
            static const size_t offset = 1;
            const size_t        length = n-offset;
            const size_t        cpu    = offset + (process_id::h32() % length);
            concurrent::nucleus::thread::assign_current(cpu,who);
            return cpu;
        }
        else
        {
            return 0;
        }
    }

}
