#include "y/seem/compiler.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(compiler)
{
    SEEM::Compiler seem(true);
    seem->GraphViz("seem.dot");

}
Y_UTEST_DONE()

