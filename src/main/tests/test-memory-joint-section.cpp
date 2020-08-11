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

    return 0;
    for(size_t i=1;i<=100000;i+=1+alea.leq(1000))
    {
        size_t       required = i;
        size_t       shift = 0;
        const size_t bytes = joint::section::bytes_to_hold(required,shift);

        std::cerr << required << " => " << bytes <<  "=2^" << shift << std::endl;
        zblock<char,global>  buffer(bytes);
        joint::section       S(buffer.rw(),bytes,shift);
        Y_ASSERT(S.size==bytes);
        Y_ASSERT(S.exp2==shift);

        void *big = S.acquire(required);
        Y_ASSERT( big );
        Y_ASSERT( &S == S.release(big,required) );
    }

    for(int iter=0;iter<16;++iter)
    {
        const size_t required = 100 + alea.leq(10000);
        size_t       shift    = 0;
        const size_t bytes    = joint::section::bytes_to_hold(required, shift);
        zblock<char,global>  buffer(bytes);
        joint::section S(buffer.rw(),bytes,shift);
        std::cerr << required << " => section.size=" << S.size << "=2^" << S.exp2 << std::endl;


        list<block> L;
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
        std::cerr << "#blocks v1=" << L.size() << std::endl;

        alea.shuffle(*L);
        while(L.size())
        {
            block     &b = L.back();
            Y_ASSERT( &S == memory::joint::section::release(b.addr,b.size) );
            L.pop_back();
        }
        Y_ASSERT(S.is_free());
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
        std::cerr << "#blocks v2=" << L.size() << std::endl;
        alea.shuffle(*L);
        for(size_t i=L.size()/2;i>0;--i)
        {
            block     &b = L.back();
            Y_ASSERT( &S == joint::section::release(b.addr,b.size) );
            L.pop_back();
        }
        while(true)
        {
            block blk = { 0, alea.range<size_t>(0,3*memory::joint::section::block::size) };
            blk.addr  = S.acquire(blk.size);
            if(!blk.addr)
            {
                break;
            }
            L.push_back(blk);
        }
        std::cerr << "#blocks v3=" << L.size() << std::endl;
        alea.shuffle(*L);
        while(L.size())
        {
            block     &b = L.back();
            Y_ASSERT( &S ==  joint::section::release(b.addr,b.size) );
            L.pop_back();
        }
        Y_ASSERT(S.is_free());
    }

    Y_UTEST_SIZEOF(joint::section::block);
    Y_UTEST_SIZEOF(joint::section);
    


}
Y_UTEST_DONE()

