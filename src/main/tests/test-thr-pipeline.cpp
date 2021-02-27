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

		explicit Worker() throw()
		{
		}

		virtual ~Worker() throw()
		{
		}

		void compute(lockable &sync)
		{
			{
				Y_LOCK(sync);
				std::cerr << "..working..2^" << Shift << std::endl;
			}
			volatile double sum = 0;
			for (size_t i = size_t(1) << Shift; i > 0; --i)
			{
				sum += 1.0 / square_of(double(i));
			}
		}

	private:
		Y_DISABLE_COPY_AND_ASSIGN(Worker);
	};

	size_t Worker::Shift = 16;

	static inline void display_env(const string &a, const string &b, void *)
	{
		std::cerr << a << '=' << b << std::endl;
	}
}

#include "y/string/convert.hpp"
#include "y/string/env.hpp"

Y_UTEST(thr_pipeline)
{
	concurrent::singleton::verbose = true;
	environment::verbose = true;
	{
		string value;
		(void)environment::get(value, Y_VERBOSE_THREADS);
	}
	concurrent::pipeline Q;
	environment::get(display_env, 0);

	{
		string value;
		(void)environment::get(value, Y_VERBOSE_THREADS);
	}

	if (argc > 1)
	{
		Worker::Shift = string_convert::to<size_t>(argv[1], "Shift");
	}


	Worker worker;
	//Q(worker, & Worker::compute );
	//Q(worker, & Worker::compute );

	real_time_clock clk;
	//clk.sleep(1);
}
Y_UTEST_DONE()

