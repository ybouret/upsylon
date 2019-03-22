#include "y/concurrent/scheme/dispatcher.hpp"
#include "y/utest/run.hpp"
#include "y/os/wtime.hpp"

using namespace upsylon;


namespace {

    class dummy
    {
    public:
        int       data;
        rt_clock &rtc;

        explicit dummy(const int d, rt_clock &c) : data(d), rtc(c)
        {

        }

        dummy( const dummy &other) : data(other.data), rtc(other.rtc)
        {
        }

        ~dummy() throw()
        {
        }

        inline void operator()( parallel &context, lockable &access )
        {
            work(context,access);
        }

        inline void work(parallel &context, lockable &access )
        {
            double dt = 0.1;
            {
                Y_LOCK(access);
                ++data;
                std::cerr << "@thread#" << context.rank << ", data=" << data <<  std::endl;
                std::cerr << "waiting..." << std::endl;
                dt += alea.to<double>()*0.3;
            }
            rtc.sleep(dt);
        }

    private:
        Y_DISABLE_ASSIGN(dummy);
    };

    
}



Y_UTEST(server)
{
    std::cerr << "sizeof(job_type)           =" << sizeof(concurrent::job_type) << std::endl;
    std::cerr << "sizeof(dispatcher::jnode)  =" << sizeof(concurrent::dispatcher::jnode) << std::endl;

    concurrent::dispatcher srv(true);

    wtime chrono;
    dummy d(7,chrono);

#if 0
    srv.enroll( &d, &dummy::work );
    srv.enroll(d);

    for(size_t i=0;i<100;++i)
    {
        srv.enroll( &d, &dummy::work );
    }

    chrono.sleep(0.5);
#endif

}
Y_UTEST_DONE()

