
#include "y/mkl/fitting/samples.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace mkl;
using namespace fitting;


Y_UTEST(fitting_sample)
{
    typedef sample<double,double,double>  dsample;
    typedef samples<double,double,double> dsamples;

    {
        const double x1[] = { 1,2,3,4 };
        const double y1[] = { 0,0.4,0.8,0.9};
        const size_t n1 = sizeof(x1)/sizeof(x1[0]);
        dsample::pointer s = dsample::create("sample1",x1,y1,n1);
    }

    dsamples  db("db");
    dsample  &s1 = db("s1");
    std::cerr << "created " << s1.name << " in " << db.name << std::endl;

}
Y_UTEST_DONE()

