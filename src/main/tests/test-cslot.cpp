#include "y/memory/cslot.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(cslot)
{

    memory::cslot<memory::global> gs;

    gs.set<float>( 1 );
    gs.set<double>( 2 );
    (void) gs.get<float>(0);
    (void) gs.get<float>(1);


}
Y_UTEST_DONE()


