
#include "y/net/comm/queues.hpp"
#include "y/type/fourcc.hpp"

#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;
using namespace net;

namespace
{
    static inline
    void testSendQ( send_queue &Q )
    {

        std::cerr << "send block_size=" << Q.data.block_size << std::endl;

        Q.write_nbo<uint32_t>( Y_FOURCC('A','B','C','D') );
        Q << " Hello, World";
        std::cerr << "sQ=" << Q << std::endl;
        while( Q.size > 0 || Q.packed() > 0)
        {
            Q.pack();
            Y_ASSERT(Q.packed()>0);
            std::cerr << "sQ=" << Q << std::endl;
            Q.update( alea.range<size_t>(1,Q.packed()) );
            std::cerr << "sQ=" << Q << std::endl;
        }
        std::cerr << std::endl;
    }


    static inline
    void testRecvQ( recv_queue &Q )
    {
        std::cerr << "recv block_size=" << Q.data.block_size << std::endl;

        Q.demoLoad("Hello, my nice little world!");
        std::cerr << Q << std::endl;
        std::cerr << std::endl;

    }

}


Y_UTEST(comm_queues)
{
    Y_UTEST_SIZEOF(send_queue);
    Y_UTEST_SIZEOF(recv_queue);

    // simulate
    for(size_t bs=1;bs<=32;bs<<=1)
    {
        {
            send_queue Q(bs);
            testSendQ(Q);
        }

        {
            recv_queue Q(bs);
            testRecvQ(Q);
        }
    }



}
Y_UTEST_DONE()



