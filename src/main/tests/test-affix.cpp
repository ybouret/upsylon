
#include "y/associative/affix/tree.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace  upsylon;

Y_UTEST(affix)
{
    Y_UTEST_SIZEOF(core::affix::tree_node);

    core::affix atree;

    int         a[4] = { 1, 2, 3, 4};
    const char *s[4] = { "hello", "hella", "hell", "happy" };
    const size_t n   = sizeof(a)/sizeof(a[0]);

    for(size_t i=0;i<n;++i)
    {
        Y_CHECK(atree.insert_with(s[i],&a[i]));
    }

    for(size_t i=0;i<n;++i)
    {
        Y_CHECK(!atree.insert_with(s[i],&a[i]));
    }


    atree.graphViz("atree.dot");


    atree.clear();



}
Y_UTEST_DONE()

