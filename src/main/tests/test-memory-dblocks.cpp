#include "y/memory/dblock.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/concurrent/sync/fake-lock.hpp"

using namespace upsylon;

static inline size_t get_block_exp2()
{
    return alea.leq(16);
}

Y_UTEST(dblocks)
{

    std::cerr << "Testing Block" << std::endl;
    for(size_t i=0;i<32;++i)
    {
        memory::dblock::pointer p( new memory::dblock(  get_block_exp2() ) );
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
            bl.push_back( new memory::dblock(   get_block_exp2() ) );
            bl.push_back( db.query( get_block_exp2() ) );
        }

        alea.shuffle(bl);
        std::cerr << db << std::endl;
    }
    std::cerr << db << std::endl;


}
Y_UTEST_DONE()

