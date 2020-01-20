#include "y/core/pool.hpp"
#include "y/utest/run.hpp"
#include "y/object.hpp"
#include "y/ptr/auto.hpp"

using namespace upsylon;

namespace {

    class dummy : public object{

    public:
        dummy *next;
        int    data;

        inline dummy() throw() : next(0), data( alea.leq(100) ) {

        }

        inline virtual ~dummy() throw()
        {

        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(dummy);
    };

}

Y_UTEST(core_pool)
{

    core::pool_of_cpp<dummy> dpool, dpool2;

    for(size_t iter=10+alea.leq(1000);iter>0;--iter)
    {
        dpool.store( new dummy() );
    }
    std::cerr << "dpool .size=" << dpool.size << std::endl;
    std::cerr << "dpool2.size=" << dpool2.size << std::endl;

    dpool.swap_with(dpool2);

    std::cerr << "dpool .size=" << dpool.size << std::endl;
    std::cerr << "dpool2.size=" << dpool2.size << std::endl;

    dpool.swap_with(dpool2);
    std::cerr << "dpool .size=" << dpool.size << std::endl;
    std::cerr << "dpool2.size=" << dpool2.size << std::endl;

    {
        const size_t n = dpool.size/2;
        while( dpool.size > n )
        {
            dpool.reverse();
            auto_ptr<dummy> tmp = dpool.query();
            Y_ASSERT( !dpool.owns( tmp.content() ) );
        }
    }

    {
        const size_t n = dpool.size/2;
        while( dpool.size > n )
        {
            dpool.pop();
        }
    }
    
    dpool.release();
    dpool.reset();

}
Y_UTEST_DONE()
