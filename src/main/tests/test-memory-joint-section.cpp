#include "y/memory/joint/section.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/memory/zblock.hpp"
#include "y/sequence/list.hpp"
#include "y/memory/allocator/global.hpp"

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

Y_UTEST(joint_section)
{

    for(size_t i=1;i<=1000;i+=1+alea.leq(9))
    {
        size_t       n = i;
        const size_t m = joint::section::bytes_to_hold(n);
        std::cerr << n << " => " << m << std::endl;
        zblock<char,global>  buffer(m);
        joint::section       S(buffer.rw(),buffer.length());
        void *big = S.acquire(n);
        Y_ASSERT( big );
        Y_ASSERT( &S == S.release(big,n) );
    }


    for(int iter=0;iter<16;++iter)
    {
        zblock<joint::section::block,global>  buffer( memory::joint::section::min_blocks  + alea.leq(1000) );
        joint::section S(buffer.rw(),buffer.length());

        std::cerr << "provided: " << buffer.length() << std::endl;
        std::cerr << "used    : " << S.bsize         << std::endl;
        std::cerr << "xsize   : " << S.xsize         << std::endl;
        std::cerr << "xexp2   : " << S.xexp2         << std::endl;

        list<block> L;
        while(true)
        {
            block blk = { 0, alea.range<size_t>(0,3*joint::section::block_size) };
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
            block blk = { 0, alea.range<size_t>(0,3*joint::section::block_size) };
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
            block blk = { 0, alea.range<size_t>(0,3*memory::joint::section::block_size) };
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

