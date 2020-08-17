
//! \file
#ifndef Y_RUN_TIME_LOG_INCLUDED
#define Y_RUN_TIME_LOG_INCLUDED 1

#include "y/concurrent/singleton.hpp"
#include "y/os/real-time-clock.hpp"
#include <iostream>
#include <iomanip>

namespace upsylon {

    //! different level of messages
    enum run_time_level
    {
        run_time_message, //!< message
        run_time_warning, //!< warning
        run_time_error    //!< error
    };

    //! run time write on std::ostream
    class run_time_log : public singleton<run_time_log>, public rt_clock
    {
    public:
        void        restart() throw(); //!< reset wall time
        double      seconds();         //!< seconds from last (re)start
        const char *ellapsed();        //!< formatted output

        std::ostream &     get(const run_time_level);        //!< get stream with header
        static const char *id(const run_time_level) throw(); //!< translation

    private:
        Y_DISABLE_COPY_AND_ASSIGN(run_time_log);
        explicit run_time_log();
        virtual ~run_time_log() throw();
        
    public:
        Y_SINGLETON_DECL_WITH(at_exit::uttermost,run_time_log); //!< setup
    };

    //! alias to run_time_log::instance().get(level)
    std::ostream & rtl( run_time_level );
    

}

#endif

