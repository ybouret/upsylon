#include "y/memory/section.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/memory/cblock.hpp"
#include "y/sequence/list.hpp"

using namespace upsylon;

Y_UTEST(section)
{
    Y_UTEST_SIZEOF(memory::section::block);
    Y_UTEST_SIZEOF(memory::section);




}
Y_UTEST_DONE()

