#include "y/net/comm/cache.hpp"
#include "y/net/comm/queues.hpp"
#include "y/net/comm/cache.hpp"
#include "y/type/fourcc.hpp"

#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/sequence/vector.hpp"
#include "y/hashing/sha1.hpp"

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
    
    static inline
    void testSendQ( send_queue &Q, const accessible<string> &strings )
    {
        std::cerr << "send block_size=" << Q.data->size << std::endl;
        Q.reset();
        comm_block::pointer blk( comm_cache::instance().query(8) );
        uint8_t     *buffer = **blk;
        const size_t buflen = blk->size;
        
        hashing::sha1 source;
        hashing::sha1 target;
        source.set();
        target.set();
        
        for(size_t i=1; i<= strings.size(); ++i)
        {
            Q << strings[i];
            source(strings[i]);
            while( Q.to_send() > 8 )
            {
                const size_t maxlen = alea.range<size_t>(1,buflen);
                const size_t curlen = Q.uploaded(buffer,maxlen);
                target.run(buffer,curlen);
            }
        }
        
        while( Q.to_send()  )
        {
            const size_t maxlen = alea.range<size_t>(1,buflen);
            const size_t curlen = Q.uploaded(buffer,maxlen);
            target.run(buffer,curlen);
        }
        
        const digest src = source.md();
        const digest tgt = target.md();
        std::cerr << "\tsrc: " << src << std::endl;
        std::cerr << "\ttgt: " << tgt << std::endl;
        Y_CHECK(src==tgt);
        std::cerr << std::endl;
    }

}

#include "y/code/base2.hpp"
#include "y/ios/icstream.hpp"

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
        // simulate
        for(size_t bx=2;bx<=10;++bx)
        {
            send_queue Q( mgr.query(bx) );
            testSendQ(Q,strings);
        }
    }
    

}
Y_UTEST_DONE()



