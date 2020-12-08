
#include "y/net/io/recv-queue.hpp"
#include "y/net/io/send-queue.hpp"

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

        Q << "Hello";
        std::cerr  << "Q=" << Q << std::endl;
        Q.pack();
        std::cerr  << "Q=" << Q << std::endl;
        std::cerr << "packed=" << Q.packed() << std::endl;
        Q.sent( Q.packed()/2 );
        std::cerr  << "Q=" << Q << std::endl;
        Q.defrag();
        std::cerr  << "Q=" << Q << std::endl;

    }
}


Y_UTEST(queues)
{
    Y_UTEST_SIZEOF(send_queue);
    Y_UTEST_SIZEOF(recv_queue);

    // simulate sending
    send_queue Q(0);
    testSendQ(Q);


}
Y_UTEST_DONE()



