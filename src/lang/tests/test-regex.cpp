#include "y/lang/pattern/compiler.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"

using namespace upsylon;
using namespace Lang;

Y_UTEST(regex)
{
    if(argc>1)
    {
        const string rx = argv[1];
        std::cerr << "Compiling '" << rx << "'" << std::endl;
        auto_ptr<Pattern> p = Compile::RegEx(rx,NULL);
        p->GraphViz("regex.dot");
    }


}
Y_UTEST_DONE()

