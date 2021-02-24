#include "y/concurrent/task/pipeline.hpp"
#include "y/concurrent/task/serial.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/os/real-time-clock.hpp"
#include "y/type/utils.hpp"

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
            {
                Y_LOCK(sync);
                std::cerr << "..working.." << std::endl;
            }
            return;
            volatile double sum = 0;
            for(size_t i=1000000000;i>0;--i)
            {
                sum += 1.0 / square_of( double(i) );
            }
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Worker);
    };

}
Y_UTEST(thr_pipeline)
{

    concurrent::pipeline Q;
    concurrent::serial   S;


    Worker worker;
    //S.enroll(worker, & Worker::compute );
    Q(worker, & Worker::compute );
    Q(worker, & Worker::compute );

    real_time_clock clk;
    clk.sleep(1);
}
Y_UTEST_DONE()

