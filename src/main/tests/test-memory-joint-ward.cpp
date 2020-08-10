
#include "y/memory/joint/ward.hpp"
#include "y/utest/run.hpp"


using namespace upsylon;
using namespace memory;

Y_UTEST(joint_ward)
{

    {
        joint::ward W(1000);
        std::cerr << W._quarry() << std::endl;
    }

    {
        joint::ward W(4000);
        std::cerr << W._quarry() << std::endl;
    }

}
Y_UTEST_DONE()

