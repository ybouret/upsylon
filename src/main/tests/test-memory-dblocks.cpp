#include "y/memory/dblock.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/concurrent/fake-lock.hpp"

using namespace upsylon;


Y_UTEST(dblocks)
{

    std::cerr << "Testing Blocks" << std::endl;
    for(size_t i=0;i<32;++i)
    {
        memory::dblock::pointer p( new memory::dblock( alea.leq(1024*1024) ) );
    }
    Y_UTEST_SIZEOF(memory::dblock);

    concurrent::fake_lock access;
    memory::dblocks       db(access);

}
Y_UTEST_DONE()

