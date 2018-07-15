#include "y/container/matrix.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(matrix)
{
    matrix_data m(3,5,4);

    std::cerr << "m.allocated=" << m.allocated << std::endl;
    std::cerr << "r_indx=" << m.r_indx << std::endl;
    std::cerr << "c_indx=" << m.c_indx << std::endl;

}
Y_UTEST_DONE()

