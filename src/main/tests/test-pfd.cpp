#include "y/mpl/pfq.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/hashing/sha1.hpp"
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
    hashing::sha1 H;
    for(size_t i=1;i<=F.size();++i)
    {
        prod.mul_by( *F[i] );
        H.set();
        prod.run(H);
        const digest   md0 = H.md();
        const mpl::pfd tmp = prod; H.set(); tmp.run(H);
        const digest   md1 = H.md();
        Y_ASSERT(md0==md1);
    }
    std::cerr << "prod=" << prod << std::endl;

    prod.ldz();
    prod.ld1();

   

}
Y_UTEST_DONE()

