
#include "y/mkl/fitting/built-in/circle.hpp"
#include "y/utest/run.hpp"


using namespace upsylon;
using namespace mkl;
using namespace fitting;


Y_UTEST(fitting_circles)
{
    
    typedef built_in::circle<unit_t,float> circle_type;
    typedef circle_type::sampling          sample_type;
    typedef circle_type::samples_type      samples_type;
    
    samples_type db("circles");
    
    sample_type &C1 = db( sample_type::create("C1") );
    sample_type &C2 = db( sample_type::create("C2") );
    
    std::cerr << C1.name << " and " << C2.name << std::endl;
    std::cerr << "in " << db.name << std::endl;
    
    
    
}
Y_UTEST_DONE()


