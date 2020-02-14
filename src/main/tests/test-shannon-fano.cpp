#include "y/information/filter/shannon-fano.hpp"

#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/string.hpp"

using namespace upsylon;
using namespace information;

#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"

Y_UTEST(shannon_fano)
{
    ShannonFano::Context ctx( Alphabet::StreamMode );

    ctx.getRoot().graphViz("sfs.dot");
}
Y_UTEST_DONE()

