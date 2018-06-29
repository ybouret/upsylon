#include "y/memory/blocks.hpp"
#include "y/utest/run.hpp"
#include "y/type/utils.hpp"

using namespace upsylon;


Y_UTEST(blocks)
{
    std::cerr << "sizeof(arena_list)=" << sizeof(memory::blocks::arena_list) << std::endl;
    std::cerr << "sizeof(arena)     =" << sizeof(memory::arena) << std::endl;
    memory::blocks b(256,400);
    
}
Y_UTEST_DONE()

