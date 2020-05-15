
#include "y/mpl/integer.hpp"
#include "y/mpl/mpn.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/list.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;


namespace {

}

Y_UTEST(mp_simplify)
{
    MPN &mp = MPN::instance();

    vector<mpn> n;
    list<mpz>   z;
    for(int i=1;i<argc;++i)
    {
        const mpn u = mpn::parse( argv[i] );
        mpz s = u;
        if( 0!=(i&1) ) s=-s;
        n.push_back(u);
        z.push_back(s);
    }


    std::cerr << "n=" << n << " = ";
    const mpn nFactor = mp.simplify(n);
    std::cerr << nFactor  << "*" << n << std::endl;
    std::cerr << "z=" << z<< " = ";
    const mpn zFactor = mp.simplify(z);
    std::cerr << zFactor  << "*" << z << std::endl;

}
Y_UTEST_DONE()
