#include "y/mpl/pfd.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;

Y_UTEST(pfd)
{
    std::cerr << "sizeof(_pfd)          = " << sizeof(mpl::_pfd)          << std::endl;
    std::cerr << "sizeof(_pfd::pointer) = " << sizeof(mpl::_pfd::pointer) << std::endl;
    std::cerr << "sizeof(_pfd::table)   = " << sizeof(mpl::_pfd::table)   << std::endl;
    std::cerr << "sizeof(pfd)           = " << sizeof(mpl::pfd)           << std::endl;

    
    vector<mpl::pfd::pointer> F(argc,as_capacity);
    for(int i=1;i<argc;++i)
    {
        const string s = argv[i];
        const mpn    n = mpn::dec(s);
        const mpl::pfd::pointer tmp = new mpl::pfd(n);
        F.push_back( tmp );
    }
    std::cerr << "F=" << F << std::endl;

    mpl::pfd prod(1);
    for(size_t i=1;i<=F.size();++i)
    {
        prod.mul_by( *F[i] );
    }
    std::cerr << "prod=" << prod << std::endl;

}
Y_UTEST_DONE()

