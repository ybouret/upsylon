#include "y/memory/section.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/memory/cblock.hpp"

using namespace upsylon;

Y_UTEST(section)
{
    Y_UTEST_SIZEOF(memory::section::block);
    Y_UTEST_SIZEOF(memory::section);

    memory::cblock_of<size_t> buffer(memory::section::min_size_t + alea.leq(100) );
    std::cerr << "#bytes=" << buffer.bytes << std::endl;
    memory::section s(buffer.data, buffer.bytes);

    s.display();

}
Y_UTEST_DONE()

