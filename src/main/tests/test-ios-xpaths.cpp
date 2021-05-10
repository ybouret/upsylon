#include "y/fs/local/fs.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/list.hpp"

using namespace upsylon;

namespace
{
    static void xtest(const char *name, const local_fs &fs)
    {
        std::cerr << name << " => ";
        list<string> which;
        if( fs.which(which,name) > 0 )
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

    //concurrent::singleton::verbose = true;
    
    local_fs &fs = local_fs::instance();

    fs.to_xpaths("PATH");
    std::cerr << "xpaths=" << fs.xpaths() << std::endl;
    xtest("dot",fs);
    xtest("ls",fs);
    xtest("dir",fs);
    
}
Y_UTEST_DONE()

