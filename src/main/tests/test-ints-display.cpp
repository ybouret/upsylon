#include "y/type/ints-display.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(ints_display)
{
    display_int::to<char>(std::cerr,-1) << std::endl;
    display_int::to<unsigned char>(std::cerr,1) << std::endl;
    display_int::to<short>(std::cerr,-1) << std::endl;
    display_int::to<unsigned short>(std::cerr,1) << std::endl;
    display_int::to<int>(std::cerr,-1) << std::endl;
    display_int::to<unsigned>(std::cerr,1) << std::endl;
    display_int::to<long>(std::cerr,-1) << std::endl;
    display_int::to<unsigned long>(std::cerr,1) << std::endl;
    display_int::to<long long>(std::cerr,-1) << std::endl;
    display_int::to<unsigned long long>(std::cerr,1) << std::endl;
}
Y_UTEST_DONE()

