#include "y/net/comm/cache.hpp"
#include "y/net/comm/queues.hpp"
#include "y/net/comm/cache.hpp"
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

        std::cerr << "send block_size=" << Q.data->size << std::endl;

        Q.write_nbo<uint32_t>( Y_FOURCC('A','B','C','D') );
        Q << " Hello, World";
        std::cerr << "sQ=" << Q << std::endl;
        while( Q.size > 0 || Q.written > 0)
        {
            Q.pack();
            Y_ASSERT(Q.written>0);
            std::cerr << "packed:  sQ=" << Q << std::endl;
            Q.update( alea.range<size_t>(1,Q.written) );
            std::cerr << "updated: sQ=" << Q << std::endl;
        }
        std::cerr << std::endl;
    }


    static inline
    void testRecvQ( recv_queue &Q )
    {
        std::cerr << "recv block_size=" << Q.data->size << std::endl;

        Q.demoLoad("Hello, my nice little world!");
        std::cerr << Q << std::endl;
        std::cerr << std::endl;

    }

}

#include "y/code/base2.hpp"

Y_UTEST(comm_queues)
{
    comm_cache &mgr = comm_cache::instance();


    // simulate
    for(size_t bx=0;bx<=5;++bx)
    {

        {
            send_queue Q( mgr.query(bx) );
            testSendQ(Q);
        }

        {
            recv_queue Q( mgr.query(bx) );
            testRecvQ(Q);
        }
    }

    std::cerr << mgr << std::endl;

    Y_UTEST_SIZEOF(send_queue);
    Y_UTEST_SIZEOF(recv_queue);

    std::cerr << "max_slots: " << base2<size_t>::max_slots << std::endl;
    

}
Y_UTEST_DONE()



