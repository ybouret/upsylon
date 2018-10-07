#include "y/codec/asf.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(asf)
{
    ASF::Alphabet alpha;
    alpha.display(std::cerr);
}
Y_UTEST_DONE()

