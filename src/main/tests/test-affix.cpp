
#include "y/associative/affix/tree.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace  upsylon;

Y_UTEST(affix)
{
    Y_UTEST_SIZEOF(core::affix::tree_node);

    core::affix atree;

    int a[4] = { 1, 2, 3, 4};

    Y_CHECK( atree.insert_with("hello",&a[0]) );
    Y_CHECK( atree.insert_with("hella",&a[1]) );
    Y_CHECK( atree.insert_with("hell",&a[3]) );

    atree.graphViz("atree.dot");





}
Y_UTEST_DONE()

