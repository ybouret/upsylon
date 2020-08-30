
#include "y/jive/regexp.hpp"
#include "y/jive/pattern/dictionary.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"

using namespace upsylon;
using namespace Jive;

Y_UTEST(jive_rx)
{
    if(argc>1)
    {
        auto_ptr<Pattern> p = RegExp(argv[1],NULL);
        p->save_to("rx.bin");
        p->graphViz("rx.dot");
    }
    
}
Y_UTEST_DONE()

