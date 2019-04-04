#include "y/mpl/pfd.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(pfd)
{
    std::cerr << "sizeof(pfd_entry)          = " << sizeof(mpl::pfd_entry)          << std::endl;
    std::cerr << "sizeof(pfd_entry::pointer) = " << sizeof(mpl::pfd_entry::pointer) << std::endl;
    std::cerr << "sizeof(pfd_table)          = " << sizeof(mpl::pfd_table)          << std::endl;
    std::cerr << "sizeof(pfd)                = " << sizeof(mpl::pfd)                << std::endl;

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

