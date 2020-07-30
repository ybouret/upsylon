#include "y/memory/section.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/memory/cblock.hpp"
#include "y/sequence/list.hpp"

using namespace upsylon;

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

Y_UTEST(section)
{
    Y_UTEST_SIZEOF(memory::section::block);
    Y_UTEST_SIZEOF(memory::section);

    for(int iter=0;iter<16;++iter)
    {
        memory::cblock_of<size_t> buffer( memory::section::min_size_t + alea.leq(1000) );
        memory::section           S(buffer.data,buffer.bytes);

        list<block> L;
        while(true)
        {
            block blk = { 0, alea.range<size_t>(0,3*memory::section::block_size) };
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
            Y_ASSERT( &S == memory::section::release(b.addr,b.size) );
            L.pop_back();
        }
        Y_ASSERT(S.is_free());
        while(true)
        {
            block blk = { 0, alea.range<size_t>(0,3*memory::section::block_size) };
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
            Y_ASSERT( &S == memory::section::release(b.addr,b.size) );
            L.pop_back();
        }
        while(true)
        {
            block blk = { 0, alea.range<size_t>(0,3*memory::section::block_size) };
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
            Y_ASSERT( &S == memory::section::release(b.addr,b.size) );
            L.pop_back();
        }
        Y_ASSERT(S.is_free());
    }


}
Y_UTEST_DONE()

