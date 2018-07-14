#include "y/associative/hash-table.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(associative)
{
    memory::__chunk<size_t> ch(10,0,0);
    std::cerr << "provided_number=" << ch.provided_number << std::endl;
}
Y_UTEST_DONE()

