#include "y/container/key-address.hpp"
#include "y/counting/perm.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace {


}

Y_UTEST(key_address)
{

    const double a[4] = { 0, 1, 2, 3 };

    std::cerr << "K1" << std::endl;
    {
        const key_address<1> K1( a[0] );
    }

    std::cerr << "K2" << std::endl;
    {
        const key_address<2> K2( a[0], a[1] );
        {
            const key_address<2> tmp(a[1],a[0]);
            Y_CHECK(tmp==K2);
        }
    }

    std::cerr << "K3" << std::endl;
    {
        const key_address<3> K3( a[0], a[1], a[2] );
        permutation          perm(3);
        for( perm.boot(); perm.good(); perm.next() )
        {
            const key_address<3> tmp( a[ perm(0) ], a[ perm(1) ], a[ perm(2) ] );
            Y_CHECK(tmp==K3);
        }
    }

    std::cerr << "K4" << std::endl;
    {
        const key_address<4> K4( a[0], a[1], a[2], a[3] );
        permutation          perm(4);
        for( perm.boot(); perm.good(); perm.next())
        {
            const key_address<4> tmp( a[ perm(0) ], a[ perm(1) ], a[ perm(2) ], a[ perm(3) ] );
            Y_CHECK(tmp==K4);
        }
    }




    
}
Y_UTEST_DONE()

