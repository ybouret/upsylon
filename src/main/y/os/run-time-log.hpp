
//! \file
#ifndef Y_RUN_TIME_LOG_INCLUDED
#define Y_RUN_TIME_LOG_INCLUDED 1

#include "y/concurrent/singleton.hpp"
#include "y/os/rt-clock.hpp"
#include <iomanip>

namespace upsylon {

    enum run_time_level
    {
        run_time_message,
        run_time_warning,
        run_time_error
    };

    class run_time_log : public singleton<run_time_log>, public rt_clock
    {
    public:
        void        restart() throw();
        double      seconds();
        const char *ellapsed();

        std::ostream & get(const run_time_level level);

        static const char *id(const run_time_level) throw();




    private:
        Y_DISABLE_COPY_AND_ASSIGN(run_time_log);
        explicit run_time_log();
        virtual ~run_time_log() throw();
        static const at_exit::longevity life_time;
        friend class singleton<run_time_log>;
    };

    std::ostream & rtl( run_time_level );
    

}

#endif

