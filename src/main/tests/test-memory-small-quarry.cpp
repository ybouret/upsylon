
#include "y/memory/small/quarry-allocator.hpp"
#include "y/memory/small/quarry.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/string/convert.hpp"
#include "y/ios/ocstream.hpp"
#include "y/concurrent/fake-lock.hpp"

#include <iomanip>

using namespace upsylon;
using namespace memory;

namespace {

    struct block
    {
        union {
            void    *entry;
            uint8_t *u8;
        };
        size_t   bytes;
        size_t   shift;
    };

    template <typename T>
    struct block_of
    {
        T     *entry;
        size_t count;
        size_t bytes;
        size_t shift;
    };

    template <typename T>
    static inline void do_test_with( small::quarry &Q )
    {
        std::cerr << std::endl;
        std::cerr << "<typed quarry_allocator>" << std::endl;
        concurrent::fake_lock   L;
        small::quarry_allocator A(L,Q);

        block_of<T>   blk[1024];
        const size_t  num = sizeof(blk)/sizeof(blk[0]);

        for(size_t i=0;i<num;++i)
        {
            blk[i].count = alea.leq(100);
            blk[i].entry = A.acquire_field<T>(blk[i].count,blk[i].bytes,blk[i].shift);
        }

        alea.shuffle(blk,num);
        for(size_t i=0;i<num;++i)
        {
            A.release_field(blk[i].entry,blk[i].count,blk[i].bytes,blk[i].shift);
        }

    }

}

Y_UTEST(small_quarry)
{
    small::quarry Q;
    block         blk[ 1024 ];
    const size_t  num = sizeof(blk)/sizeof(blk[0]);

    {


        for(size_t i=0;i<num;++i)
        {
            block &b = blk[i];
            b.shift  = alea.leq(16);
            b.entry  = Q.pull(b.shift);
        }
        std::cerr << Q << std::endl;


        alea.shuffle(blk,num);
        for(size_t i=0;i<num;++i)
        {
            block &b = blk[i];
            Q.push(b.entry,b.shift);
        }

        std::cerr << Q << std::endl;
        if(false)
        {
            static const unsigned ll[] =
            {
                0,0,0,0,2,2,2,3,3,3,4,4,4,4,5,5,5,6,6,6,7,7,7,7,8,8,8,9,9,9,10,10,10,10,11,11,11,12,12,12,13,13,13,13,14,14,14,15,15,15,16,16,16,16,17,17,17,18,18,18,19,19,19,19
            };

            std::cerr << "#ll=" << sizeof(ll)/sizeof(ll[0]) << std::endl;
            ios::ocstream fp1("qlen.dat");
            ios::ocstream fp2("qsup.dat");
            size_t p = 0;
            for(size_t i=0;i<small::vein::min_exp2;++i)
            {
                std::cerr << "0,";
            }
            for(size_t i=small::vein::min_exp2;i<=small::vein::max_exp2;++i)
            {
                char buffer[1024] = { 0 };
                memset(buffer,0,sizeof(buffer));
                snprintf(buffer, sizeof(buffer)-1, "%lu", (unsigned long) Q[i].block_size );
                const size_t n = strlen(buffer);
                //std::cerr << i << "->" << n << " (" << buffer << ")" << std::endl;
                fp1("%u %u\n", unsigned(i), unsigned(n) );
                Y_ASSERT(n==ll[i]);
                if(n>p)
                {
                    fp2("%u %u\n", unsigned(i), unsigned(n) );
                    p=n;
                }
                std::cerr << n;
                if(i<small::vein::max_exp2) std::cerr << ",";
            }
            std::cerr << std::endl;
        }
    }

    {
        concurrent::fake_lock   L;
        small::quarry_allocator A(L,Q);

        for(size_t i=0;i<num;++i)
        {
            block &b = blk[i];
            b.bytes  = i>0 ? alea.leq(1000) : 0;
            b.u8     = A.acquire_bytes(b.bytes,b.shift);
            Y_ASSERT(1<<b.shift==b.bytes);
        }
        alea.shuffle(blk,num);

        for(size_t i=0;i<num;++i)
        {
            block &b = blk[i];
            A.release_bytes(b.u8, b.bytes, b.shift);
        }

    }

    {
        do_test_with<uint8_t>(Q);
        do_test_with<float>(Q);
        do_test_with<double>(Q);
    }





    std::cerr << Q << std::endl;


}
Y_UTEST_DONE()
