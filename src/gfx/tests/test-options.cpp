#include "y/gfx/image/format/options.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace graphic;

Y_UTEST(options)
{
    image::options opts("hello=true");
    opts.parse("alpha=1:z=6");

    std::cerr << "<options>" << std::endl;
    for(image::options::iterator it=opts.begin();it!=opts.end();++it)
    {
        std::cerr << '\t' << **it << std::endl;
    }
    std::cerr << "<options/>" << std::endl;


}
Y_UTEST_DONE()
