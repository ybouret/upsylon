#include "y/core/pool.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace {

    class dummy {

    public:
        dummy *next;
        int    data;

        inline dummy() throw() : next(0), data( alea.leq(100) ) {

        }

        inline ~dummy() throw()
        {

        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(dummy);
    };

}

Y_UTEST(core_pool)
{

    core::pool_of_cpp<dummy> dpool;
    for(size_t iter=10+alea.leq(1000);iter>0;--iter)
    {
        dpool.store( new dummy() );
    }
    std::cerr << "dpool.size=" << dpool.size << std::endl;


}
Y_UTEST_DONE()
