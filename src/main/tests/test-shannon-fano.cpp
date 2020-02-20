#include "y/information/translator/shannon-fano.hpp"

#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/string.hpp"

using namespace upsylon;
using namespace information;

#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"

Y_UTEST(shannon_fano)
{
    ShannonFano::Tree tree;

    tree.update('a');
    tree.update('b');
    tree.update('a');
    tree.displayChars();

}
Y_UTEST_DONE()

