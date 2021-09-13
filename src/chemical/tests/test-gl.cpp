
#include "y/utest/run.hpp"
#include "y/mkl/kernel/gram-schmidt.hpp"

using namespace upsylon;
using namespace mkl;

Y_UTEST(gl)
{
    {
        matrix<int> GS(3,3);
        // Nu
        GS[1][1] = 1; GS[1][2] = -1; GS[1][3] = 1;

        // conservation
        GS[2][1] = 0; GS[2][2] = 1; GS[2][3] = 1;

        GS[3][1] = 1; GS[3][2] = 0; GS[3][3] = -1;
        std::cerr << "gs=" << GS <<  std::endl;

        if(!GramSchmidt::iOrtho(GS)) throw exception("cannot orthor GS");
        std::cerr << "gso=" << GS << std::endl;
    }
}
Y_UTEST_DONE()

