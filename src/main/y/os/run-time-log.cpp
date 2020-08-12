
#include "y/os/run-time-log.hpp"
#include "y/type/ints.hpp"
#include "y/os/progress.hpp"
#include <cstring>
#include <iostream>

namespace upsylon
{
    namespace {

        static uint64_t rtl_mark = 0;
        static char     rtl_time[ progress::format_size ];
        std::ostream   *rtl_os   = 0;
    }

    Y_SINGLETON_IMPL(run_time_log);

    run_time_log:: run_time_log() : rt_clock()
    {
        restart();
        rtl_mark = ticks();
        rtl_os   = & std::cerr;
    }

    run_time_log:: ~run_time_log() throw()
    {
        restart();
    }

    void run_time_log:: restart() throw()
    {
        rtl_mark = 0;
        memset(rtl_time,0,sizeof(rtl_time));
    }

    double run_time_log:: seconds()
    {
        const uint64_t now = ticks();
        return (*this)(now-rtl_mark);
    }

    const char * run_time_log:: id(const run_time_level level) throw()
    {
        switch(level)
        {
            case run_time_message: return "message";
            case run_time_warning: return "warning";
            case run_time_error:   return "*error*";
        }
        return "???";
    }

    const char *run_time_log:: ellapsed()
    {
        progress::format(rtl_time,seconds());
        return rtl_time;
    }

    std::ostream & run_time_log:: get(const run_time_level level)
    {
        assert(rtl_os);
        std::ostream &os = *rtl_os;
        os << "***";
        os << '[' << ellapsed() << ']';
        os << '<' << id(level)  << '>';
        os << ' ';
        return os;
    }

    std::ostream & rtl(const run_time_level level)
    {
        static run_time_log &self = run_time_log::instance();
        return self.get(level);
    }
    



}

