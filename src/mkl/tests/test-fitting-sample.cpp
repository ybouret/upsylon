
#include "y/mkl/fitting/samples.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace mkl;
using namespace fitting;


Y_UTEST(fitting_sample)
{
    {
        const double x1[] = { 1,2,3,4 };
        const double y1[] = { 0,0.4,0.8,0.9};
        const size_t n1 = sizeof(x1)/sizeof(x1[0]);
        sample<double,double>::pointer s = sample<double,double>::create("sample1",x1,y1,n1);
    }

    samples<double,double> db("db");
    

}
Y_UTEST_DONE()

