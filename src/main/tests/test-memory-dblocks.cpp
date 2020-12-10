#include "y/memory/dblock.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/concurrent/fake-lock.hpp"

using namespace upsylon;

static inline size_t get_block_size()
{
    return alea.leq(1024*1024);
}

Y_UTEST(dblocks)
{

    std::cerr << "Testing Block" << std::endl;
    for(size_t i=0;i<32;++i)
    {
        memory::dblock::pointer p( new memory::dblock(  get_block_size() ) );
    }
    Y_UTEST_SIZEOF(memory::dblock);
    std::cerr << std::endl;

    std::cerr << "Testing Blocks" << std::endl;
    concurrent::fake_lock access;
    memory::dblocks       db(access);

    {
        memory::dblock::list bl;

        for(size_t i=0;i<32;++i)
        {
            bl.push_back( new memory::dblock(   get_block_size() ) );
            bl.push_back( db.query( get_block_size() ) );
        }

        alea.shuffle(bl);
        std::cerr << db << std::endl;
    }
    std::cerr << db << std::endl;


}
Y_UTEST_DONE()

