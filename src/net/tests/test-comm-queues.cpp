
#include "y/net/comm/recv-queue.hpp"
#include "y/net/comm/send-queue.hpp"

#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;
using namespace net;

namespace
{
    static inline
    void testSendQ( send_queue &Q )
    {

        std::cerr << "block_size=" << Q.block_size() << std::endl;

        Q << "Hello, World";
        std::cerr << "Q=" << Q << std::endl;
        while( Q.size > 0 || Q.packed() > 0)
        {
            Q.pack();
            Y_ASSERT(Q.packed()>0);
            std::cerr << "Q=" << Q << std::endl;
            Q.update( alea.range<size_t>(1,Q.packed()) );
            std::cerr << "Q=" << Q << std::endl;
        }

    }
}


Y_UTEST(comm_queues)
{
    Y_UTEST_SIZEOF(send_queue);
    Y_UTEST_SIZEOF(recv_queue);

    // simulate sending
    send_queue Q(0);
    testSendQ(Q);


}
Y_UTEST_DONE()



