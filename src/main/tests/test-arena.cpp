#include "y/memory/arena-of.hpp"
#include "y/utest/run.hpp"
#include "y/memory/zblock.hpp"
#include "y/memory/allocator/global.hpp"
#include <typeinfo>

using namespace upsylon;
namespace
{
    struct block
    {
        void *addr;
    };
}

template <typename T>
static inline void test_arena_of(const size_t nb)
{
    const char *tid = typeid(T).name();
    std::cerr << "< arena_of<" << tid << "> >" << std::endl;
    memory::arena_of<T>        A( 4096 );
    zblock<T*,memory::global>  blk(nb);

    const size_t n   = blk.count;
    const size_t h   = n/2;
    for(size_t i=1;i<=n;++i)
    {
        blk[i] = A.acquire();
    }
    alea.shuffle( &blk[1],n);
    for(size_t i=h;i<=n;++i)
    {
        A.release(blk[i]);
    }
    for(size_t i=h;i<=n;++i)
    {
        blk[i] = A.acquire();
    }
    alea.shuffle( &blk[1],n);

    for(size_t i=1;i<=n;++i)
    {
        A.release(blk[i]);
    }
    std::cerr << "< arena_of<" << tid << ">/>" << std::endl;

}

Y_UTEST(arena)
{

    
    const size_t nb = 4000;

    zblock<block,memory::global> blocks(nb);
    const size_t  n   = blocks.count;
    const size_t  h   = n/2;
    block       * blk = *blocks;

    for(size_t block_size=1;block_size<=128;block_size*=2)
    {
        std::cerr << "block_size=" << block_size << std::endl;
        for(size_t chunk_size=1;chunk_size<=4096;chunk_size*=2)
        {
            //std::cerr << "\tchunk_size=" << chunk_size << std::endl;
            std::cerr << "/" << chunk_size;
            memory::arena A(block_size,chunk_size);
            std::cerr << "->(" << A.chunk_size <<"," << A.chunks_per_page << ")";
            for(size_t i=0;i<n;++i)
            {
                blk[i].addr = A.acquire();
            }
            alea.shuffle(blk,n);
            for(size_t i=0;i<h;++i)
            {
                A.release(blk[i].addr);
            }
            for(size_t i=0;i<h;++i)
            {
                blk[i].addr = A.acquire();
            }
            alea.shuffle(blk,n);
            for(size_t i=0;i<n;++i)
            {
                Y_ASSERT( A.owns(blk[i].addr) );
                A.release(blk[i].addr);
            }
        }
        std::cerr << std::endl;
    }

    std::cerr << "sizeof(memory::arena)=" << sizeof(memory::arena) << std::endl;
    std::cerr << "sizeof(memory::chunk)=" << sizeof(memory::chunk) << std::endl;

    test_arena_of<char>(nb*2);
    test_arena_of<int>(nb*2);
    test_arena_of<uint64_t>(nb*2);



}
Y_UTEST_DONE()

