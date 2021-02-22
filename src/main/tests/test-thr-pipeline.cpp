#include "y/concurrent/task/pipeline.hpp"
#include "y/concurrent/task/serial.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/os/real-time-clock.hpp"

using namespace upsylon;

namespace {

    class Worker
    {
    public:

        explicit Worker() throw()
        {
        }

        virtual ~Worker() throw()
        {
        }

        void compute( lockable &sync )
        {
            Y_LOCK(sync);
            std::cerr << "..working.." << std::endl;
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Worker);
    };

}
Y_UTEST(thr_pipeline)
{

    concurrent::pipeline Q;
    concurrent::serial   S;

    Y_UTEST_SIZEOF(concurrent::pipeline::task);

    Worker worker;
    S.enroll(worker, & Worker::compute );
    Q.enroll(worker, & Worker::compute );

    real_time_clock clk;
    clk.sleep(1);

}
Y_UTEST_DONE()

