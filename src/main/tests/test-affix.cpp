
#include "y/associative/affix/tree.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace  upsylon;

Y_UTEST(affix)
{
    Y_UTEST_SIZEOF(core::affix::tree_node);

    core::affix atree;

    atree.graphViz("atree.dot");


}
Y_UTEST_DONE()

