#include "y/concurrent/scheme/dispatcher.hpp"
#include "y/utest/run.hpp"
#include "y/os/wtime.hpp"

using namespace upsylon;


namespace {

    class dummy
    {
    public:
        int data;

        explicit dummy(const int d) : data(d)
        {

        }

        dummy( const dummy &other) : data(other.data)
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
            Y_LOCK(access);
            ++data;
            std::cerr << "@thread#" << context.rank << ", data=" << data <<  std::endl;
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

    dummy d(7);

    wtime chrono;

    srv.enroll( &d, &dummy::work );
    srv.enroll(d);

    chrono.sleep(0.5);


}
Y_UTEST_DONE()

