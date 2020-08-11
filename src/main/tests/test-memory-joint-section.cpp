#include "y/memory/joint/section.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/memory/zblock.hpp"
#include "y/sequence/list.hpp"
#include "y/memory/allocator/global.hpp"
#include <iomanip>

using namespace upsylon;
using namespace memory;

namespace
{
    struct block
    {
        void  *addr;
        size_t size;
#if 0
        inline friend std::ostream & operator<<( std::ostream &os, const block &b )
        {
            os << "<@" << b.addr << "+" << b.size << ">";
            return os;
        }
#endif
    };
}

#define SHOW(FIELD) std::cerr << "section:: " << std::left << std::setw(16) <<  #FIELD << " = " << joint::section::FIELD << std::endl

Y_UTEST(joint_section)
{

    SHOW(block::size);
    SHOW(block::exp2);
    SHOW(min_blocks);
    SHOW(min_size_);
    SHOW(min_vein_);
    SHOW(min_size);
    SHOW(min_exp2);
    SHOW(max_size);
    SHOW(max_exp2);
    SHOW(min_allocated);
    SHOW(max_allocated);

    std::cerr << "-- one shot acquire" << std::endl;
    for(size_t i=1;i<=100000;i+=1+alea.leq(8000))
    {
        size_t       required = i;
        size_t       shift = 0;
        const size_t bytes = joint::section::holding(required,shift);

        zblock<char,global>  buffer(bytes);
        joint::section       S(buffer.rw(),bytes,shift);
        Y_ASSERT(S.size==bytes);
        Y_ASSERT(S.exp2==shift);
        std::cerr << required << " => " << bytes <<  "=2^" << shift << " => capacity=" << S.capacity << std::endl;

        void *big = S.acquire(required);
        Y_ASSERT( big );
        Y_ASSERT( &S == S.release(big,required) );
    }
    std::cerr << std::endl;

    std::cerr << "-- multiple I/O" << std::endl;
    list<block> L;
    for(int iter=0;iter<32;++iter)
    {
        const size_t required = 100 + alea.leq(10000);
        size_t       shift    = 0;
        const size_t bytes    = joint::section::holding(required, shift);
        zblock<char,global>  buffer(bytes);
        joint::section S(buffer.rw(),bytes,shift);
        std::cerr << std::setw(6) << required << " => section.size=" << std::setw(6) << S.size << " = 2^" << std::setw(2) << S.exp2;
        std::cerr.flush();

        size_t maxBlocks=0;
        std::cerr << ' ' << '[';
        for(size_t cycle=1;cycle<=256;++cycle)
        {
            if(0==(cycle%16)) std::cerr << '.';
            assert(0==L.size());
            while(true)
            {
                block blk = { 0, alea.range<size_t>(0,3*joint::section::block::size) };
                blk.addr  = S.acquire(blk.size);
                if(!blk.addr)
                {
                    break;
                }
                L.push_back(blk);
            }
            if(L.size()>maxBlocks) maxBlocks=L.size();
            alea.shuffle(*L);
            while(L.size())
            {
                block     &b = L.back();
                Y_ASSERT( &S == memory::joint::section::release(b.addr,b.size) );
                L.pop_back();
            }
            Y_ASSERT(S.is_empty());
        }
        std::cerr << ']';
        std::cerr << " max blocks = " << std::setw(4) << maxBlocks;
        std::cerr << std::endl;
    }

    Y_UTEST_SIZEOF(joint::section::block);
    Y_UTEST_SIZEOF(joint::section);
    


}
Y_UTEST_DONE()

