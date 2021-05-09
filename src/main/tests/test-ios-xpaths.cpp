#include "y/ios/tools/xpaths.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/list.hpp"

using namespace upsylon;

namespace
{
    static void xtest(const char *name, const xpaths &xp)
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

Y_UTEST(xpaths)
{
    
    xpaths xp;
    xp.load("PATH");
    std::cerr << "xpaths=" << xp << std::endl;

    xtest("dot",xp);
    xtest("ls",xp);
    xtest("dir",xp);
    
}
Y_UTEST_DONE()

