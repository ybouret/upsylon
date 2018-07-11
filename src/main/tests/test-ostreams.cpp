#include "y/ios/ocstream.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace
{

}

Y_UTEST(ostreams)
{
    {
        const string fn = "echo.dat";
        ios::ocstream::overwrite(fn);
        int i=0;
        ios::ocstream::echo(fn,"Hello, %d\n",++i);
        ios::ocstream::echo(fn,"Hello, %d\n",++i);
        ios::ocstream::echo(fn,"Hello, %d\n",++i);
        ios::ocstream fp(fn,true);
        fp << fn;
        fp << '\n';
        fp << "Hello, World!" << "\n";
        fp("Hello, %d\n", ++i);
    }
}
Y_UTEST_DONE()


