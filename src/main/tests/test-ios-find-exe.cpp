#include "y/ios/tools/find-executable.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/list.hpp"

using namespace upsylon;

namespace
{
    static void xtest(const char *name, const exe_paths &xp)
    {
        std::cerr << name << " => ";
        list<string> which;
        if( xp(which,name) > 0 )
        {
            std::cerr << which << std::endl;
        }
        else
        {
            std::cerr << "not found" << std::endl;
        }
    }
}

Y_UTEST(find_exe)
{
    
    exe_paths xpaths;
    xpaths.load("PATH");
    std::cerr << "xpaths=" << xpaths << std::endl;

    xtest("dot",xpaths);
    xtest("ls",xpaths);
    xtest("dir",xpaths);
    
}
Y_UTEST_DONE()

