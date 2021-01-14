
#include "y/mkl/fitting/sample/load.hpp"

#include "y/utest/run.hpp"

using namespace upsylon;
using namespace mkl;
using namespace fitting;


Y_UTEST(fitting_load)
{
    typedef sample<double,double>  dsample;

    dsample::pointer s = dsample::create("loaded", 1024);

    if(argc>1)
    {
        const size_t loaded = load_sample::from_file(argv[1], *s, 1, 2);
        std::cerr << "#data=" << loaded << std::endl;
    }


}
Y_UTEST_DONE()
