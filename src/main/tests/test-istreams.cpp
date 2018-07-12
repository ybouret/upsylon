
#include "y/ios/icstream.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(istreams)
{
    ios::icstream fp( ios::cstdin );
    string line;
    while( fp.gets(line) )
    {
        std::cerr << line << std::endl;
    }
}
Y_UTEST_DONE()
