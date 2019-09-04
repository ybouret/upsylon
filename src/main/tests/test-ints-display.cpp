#include "y/type/ints-display.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(ints_display)
{
    core::display_int<char>(std::cerr,-1) << std::endl;
    core::display_int<unsigned char>(std::cerr,1) << std::endl;
    core::display_int<short>(std::cerr,-1) << std::endl;
    core::display_int<unsigned short>(std::cerr,1) << std::endl;
    core::display_int<int>(std::cerr,-1) << std::endl;
    core::display_int<unsigned>(std::cerr,1) << std::endl;
    core::display_int<long>(std::cerr,-1) << std::endl;
    core::display_int<unsigned long>(std::cerr,1) << std::endl;
    core::display_int<long long>(std::cerr,-1) << std::endl;
    core::display_int<unsigned long long>(std::cerr,1) << std::endl;
}
Y_UTEST_DONE()

