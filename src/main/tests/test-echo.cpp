#include "y/ios/echostream.hpp"
#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;

Y_UTEST(echo)
{
    ios::echostream fp;

    fp.link( new ios::ocstream("echo1.dat")  );
    fp.link( new ios::ocstream("echo2.dat")  );
    fp.link( new ios::ocstream(ios::cstderr) );

    fp << "Hello, World!\n";


}
Y_UTEST_DONE()

