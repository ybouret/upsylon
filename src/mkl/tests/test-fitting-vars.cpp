#include "y/mkl/fitting/variables.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace mkl;
using namespace fitting;

Y_UTEST(fitting_vars)
{

    {
        const shared_variable v_D  = new primary_variable("D",1);
        const shared_variable v_t1 = new primary_variable("t1",2);
        const shared_variable v_t2 = new primary_variable("t2",3);

        const shared_variable v_t0a = new replica_variable("t0",v_t1);
        const shared_variable v_t0b = new replica_variable("t0",v_t2);

        std::cerr << v_D   << std::endl;
        std::cerr << v_t1  << std::endl;
        std::cerr << v_t2  << std::endl;
        std::cerr << v_t0a << std::endl;
        std::cerr << v_t0b << std::endl;


    }

}
Y_UTEST_DONE()
