#include "y/codec/iobits.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(iobits)
{
    std::cerr << "sizeof(iobit)=" << sizeof(iobit) << std::endl;
    iobits Q;
    for(size_t i=1000+alea.leq(10000);i>0;--i)
    {
        Q.push( alea.choice() );
    }

    Q.free();
    Q.pool.keep(256);

}
Y_UTEST_DONE()

