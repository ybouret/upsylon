#include "y/type/bitwise.hpp"
#include "y/utest/run.hpp"

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



}
Y_UTEST_DONE()

