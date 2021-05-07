
#include "y/os/run-time-log.hpp"
#include "y/type/ints.hpp"
#include "y/os/progress.hpp"
#include "y/code/human-readable.hpp"
#include <cstring>
#include <iostream>
#include <cmath>

namespace upsylon
{
    namespace {

        static uint64_t rtl_mark = 0;
        std::ostream   *rtl_os   = 0;
    }

    Y_SINGLETON_IMPL(run_time_log);

    run_time_log:: run_time_log() : real_time_clock()
    {
        restart();
        rtl_os   = & std::cerr;
    }

    run_time_log:: ~run_time_log() throw()
    {
        restart();
    }

    void run_time_log:: restart() throw()
    {
        rtl_mark = ticks();
    }

    double run_time_log:: seconds()
    {
        const uint64_t now = ticks();
        return (*this)(now-rtl_mark);
    }


    uint64_t run_time_log:: micro_s()
    {
        const uint64_t now = ticks();
        const double ell = (*this)(now-rtl_mark);
        return uint64_t( floor(1.0e6 * ell +0.5) );
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

    

    std::ostream & run_time_log:: get(const run_time_level level)
    {
        assert(rtl_os);
        std::ostream &os = *rtl_os;
        os << "***";
        os << '[' << human_readable( micro_s() ) << "mu" << ']';
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

