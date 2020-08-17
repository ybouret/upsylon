#include "y/os/run-time-log.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(rtl)
{
    real_time_clock &clk = run_time_log::instance();

    rtl(run_time_message) << " hello! " << std::endl;
    clk.sleep(0.2);
    rtl(run_time_warning) << " hello! " << std::endl;
    clk.sleep(0.4);
    rtl(run_time_error)   << " hello! " << std::endl;

}
Y_UTEST_DONE()




