#include "y/mpl/pfd.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(pfd)
{
    std::cerr << "sizeof(_pfd)          = " << sizeof(mpl::_pfd)          << std::endl;
    std::cerr << "sizeof(_pfd::pointer) = " << sizeof(mpl::_pfd::pointer) << std::endl;
    std::cerr << "sizeof(_pfd::table)   = " << sizeof(mpl::_pfd::table)   << std::endl;
    std::cerr << "sizeof(pfd)           = " << sizeof(mpl::pfd)           << std::endl;

    if( argc > 1 )
    {
        const string s = argv[1];
        const mpn    n = mpn::dec(s);
        std::cerr << "n=" << n << std::endl;
        mpl::pfd     x(n);
        std::cerr << "x.table=" << x.table << std::endl;
    }


}
Y_UTEST_DONE()

