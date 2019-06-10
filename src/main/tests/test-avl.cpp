#include "y/ordered/avl-tree.hpp"
#include "y/string.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(avl)
{

    avl::tree<int>    itree;
    avl::tree<string> stree;

    itree.reserve(100);
    stree.reserve(1000);


}
Y_UTEST_DONE()



