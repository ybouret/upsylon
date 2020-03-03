#include "y/type/bitwise.hpp"
#include "y/type/point2d.hpp"
#include "y/utest/run.hpp"
#include "y/type/xswap.hpp"

using namespace upsylon;

namespace {
    struct dummy
    {
        int a;
    };

    
}

Y_UTEST(traits)
{
    Y_CHECK( type_traits<int>::is_const       == false );
    Y_CHECK( type_traits<const int>::is_const == true );
    Y_CHECK( type_traits<dummy>::is_const       == false );
    Y_CHECK( type_traits<const dummy>::is_const == true );

    Y_CHECK( no_throw_copy<double>::value == true );
    Y_CHECK( no_throw_copy< point2d<float> >::value == true );
    Y_CHECK( no_throw_copy< complex<double> >::value == true );


}
Y_UTEST_DONE()

