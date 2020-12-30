#include "y/mkl/fitting/variables.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;
using namespace mkl;
using namespace fitting;

namespace
{

}

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

    variables vars;
    vars << "D" << "t1" << "t2";
    std::cerr << "vars: " << vars << std::endl;

    variables sub1;
    sub1("D",vars("D"));
    sub1("t0",vars("t1"));
    std::cerr << "sub1: " << sub1 << std::endl;

    variables sub2;
    sub2("D",vars("D"));
    sub2("t0",vars("t2"));
    std::cerr << "sub2: " << sub2 << std::endl;

    const size_t   n = vars.sweep();
    vector<int>    aorg(n,0);

    const variable &D = vars["D"];
    {
        const int r = alea.full<int>();
        D(aorg) = r;
        Y_CHECK(r==vars(aorg,"D"));
        Y_CHECK(r==sub1(aorg,"D"));
        Y_CHECK(r==sub2(aorg,"D"));
    }

    vars.make(aorg, "t1:t2", 1);
    std::cerr << "aorg=" << aorg << std::endl;





}
Y_UTEST_DONE()
