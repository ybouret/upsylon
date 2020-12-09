#include "y/net/comm/bytes.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;
using namespace net;

namespace {

    static inline
    void display( const comm_bytes &Q )
    {
        std::cerr << "size: " << Q.size << " pool: " << Q.pool.size << std::endl;
        std::cerr << "[" << Q << "]" << std::endl;
    }

}

Y_UTEST(comm_bytes)
{

    comm_byte::supply &mgr = comm_byte::instance();
    std::cerr << "mgr.prefetch=" << mgr.prefetched() << std::endl;
    {
        comm_bytes Q;
        display(Q);
        Q.push("Hello !");
        display(Q);
        Q.clear();
        display(Q);
        Q.push("World");
        display(Q);
    }
    std::cerr << "mgr.prefetch=" << mgr.prefetched() << std::endl;
    
}
Y_UTEST_DONE()



