#include "y/concurrent/scheme/dispatcher.hpp"
#include "y/utest/run.hpp"

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
            std::cerr << context.indx << ", data=" << data <<  std::endl;
        }

    private:
        Y_DISABLE_ASSIGN(dummy);
    };

    
}



Y_UTEST(server)
{
    std::cerr << "sizeof(job_type)           =" << sizeof(concurrent::job_type) << std::endl;
    std::cerr << "sizeof(dispatcher::jnode)  =" << sizeof(concurrent::dispatcher::jnode) << std::endl;

    concurrent::dispatcher d(true);
    


}
Y_UTEST_DONE()

