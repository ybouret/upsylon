
#include "y/jive/regexp.hpp"
#include "y/jive/pattern/dictionary.hpp"
#include "y/jive/pattern/all.hpp"
#include "y/jive/pattern/posix.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"

using namespace upsylon;
using namespace Jive;

Y_UTEST(jive_rx)
{
    Dictionary dict;
    if(!dict.insert( "INT", Repeating::Create( posix::digit(), 1) ))
    {
        throw exception("couldn't insert INT");
    }
    if(argc>1)
    {
        auto_ptr<Pattern> p = RegExp(argv[1],&dict);
        p->save_to("rx.bin");
        p->graphViz("rx.dot");
        const string expr = p->toRegExp();
        std::cerr << "expr='" << expr << "'" << std::endl;
        auto_ptr<Pattern> q = RegExp(expr,0);
        q->save_to("rx2.bin");
        q->graphViz("rx2.dot");
        Y_CHECK(*p==*q);
    }
    
}
Y_UTEST_DONE()

