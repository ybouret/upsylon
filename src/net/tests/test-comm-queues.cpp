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

        std::cerr << "sQ=" << Q << std::endl;
        Q.write_nbo<uint32_t>( Y_FOURCC('A','B','C','D') );
        std::cerr << "sQ=" << Q << std::endl;
        Q << " Hello, World";
        std::cerr << "sQ=" << Q << std::endl;
        Q << " from Mars";
        std::cerr << "sQ=" << Q << std::endl;
        Q.remove( Q.readable/2 );
        std::cerr << "sQ=" << Q << std::endl;

#if 0
        while( Q.size > 0 || Q.written > 0)
        {
            Q.pack();
            Y_ASSERT(Q.written>0);
            std::cerr << "packed:  sQ=" << Q << std::endl;
            Q.update( alea.range<size_t>(1,Q.written) );
            std::cerr << "updated: sQ=" << Q << std::endl;
        }
#endif
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
#include "y/ios/icstream.hpp"
#include "y/sequence/vector.hpp"

Y_UTEST(comm_queues)
{
    comm_cache &mgr = comm_cache::instance();


    // simulate
    for(size_t bx=2;bx<=4;++bx)
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
    
    if(argc>1)
    {
        vector<string> strings;
        {
            ios::icstream  fp(argv[1]);
            string         line;
            while( fp.gets(line) ) strings << line;
        }
        std::cerr << "#lines=" << strings.size() << std::endl;
    }
    

}
Y_UTEST_DONE()



