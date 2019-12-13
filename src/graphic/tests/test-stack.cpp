#include "y/graphic/stack.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Graphic;

Y_UTEST(stack)
{
    Stack<float> pxms(3,200,100);
    for(size_t i=0;i<pxms.count;++i)
    {
        Y_ASSERT(pxms[i]->sameAreaThan(pxms));
    }
}
Y_UTEST_DONE()


