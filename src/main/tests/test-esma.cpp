#include "y/string/esma.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(esma)
{

    { core::esma mgr; }

    {
        core::esma mgr;

        mgr.prepare(0, 1);
        for(size_t iter=0;iter<1024;++iter)
        {
            const size_t numChars = alea.leq(256);
            const size_t charSize = 1+alea.leq(11);
            mgr.prepare(numChars,charSize);
        }
    }

}
Y_UTEST_DONE()

