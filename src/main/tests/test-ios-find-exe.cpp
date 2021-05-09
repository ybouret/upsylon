#include "y/ios/tools/find-executable.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(find_exe)
{
    
    exe_paths xpaths;
    xpaths.load("PATH");
    std::cerr << "xpaths=" << xpaths << std::endl;
}
Y_UTEST_DONE()

