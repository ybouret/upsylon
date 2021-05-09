#include "y/ios/tools/find-executable.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(find_exe)
{
    
    find_exe::strings paths;
    find_exe::load_paths(paths, "PATH");
    std::cerr << paths << std::endl;
}
Y_UTEST_DONE()

