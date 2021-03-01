#include "y/concurrent/task/pipeline.hpp"
#include "y/concurrent/task/serial.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/os/real-time-clock.hpp"
#include "y/type/utils.hpp"
#include "y/string.hpp"

using namespace upsylon;

namespace {

	class Worker
	{
	public:
		static size_t Shift;

        double total;

		explicit Worker() throw() : total(0)
		{
		}

        Worker(const Worker &other) throw() : total(other.total)
        {
        }

		virtual ~Worker() throw()
		{
		}

		void compute(lockable &sync)
		{
			{
				Y_LOCK(sync);
				std::cerr << "<working..2^" << Shift << ">" << std::endl;
			}
			volatile double sum = 0;
			for (size_t i = size_t(1) << Shift; i > 0; --i)
			{
				sum += 1.0 / square_of(double(i));
			}
            total = sum;
		}

	private:
        Y_DISABLE_ASSIGN(Worker);
	};

	size_t Worker::Shift = 16;

	 
}

#include "y/string/convert.hpp"

Y_UTEST(thr_pipeline)
{
    
	concurrent::pipeline Q;
    size_t               works = 1;
    

	if (argc > 1)
	{
		Worker::Shift = string_convert::to<size_t>(argv[1], "Shift");
	}

    if(argc>2)
    {
        works = string_convert::to<size_t>(argv[2],"works");
    }

    Worker worker;
    for(size_t i=1;i<=works;++i)
    {
        Q(worker, & Worker::compute );
    }


    Q.flush();

    return 0;

	real_time_clock clk;
    clk.sleep(1);
}
Y_UTEST_DONE()

