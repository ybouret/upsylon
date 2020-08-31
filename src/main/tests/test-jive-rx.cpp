
#include "y/jive/pattern/dictionary.hpp"
#include "y/jive/pattern/all.hpp"
#include "y/jive/pattern/posix.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"

using namespace upsylon;
using namespace Jive;

Y_UTEST(jive_rx)
{
    if(false)
    {
        auto_ptr<Pattern> p = posix::endl();
        p->graphViz("endl.dot");
        auto_ptr<And> q = And::Create();
        q->push_back( p.yield() );
        q->optimize();
        q->graphViz("endl2.dot");
    }
    Dictionary        dict;
    RegExpVerbose() = true;
    if(!dict.use("DIGIT","[:digit:]"))
    {
        throw exception("couldn't insert DIGIT");
    }
    if(!dict.use("INT","{DIGIT}+"))
    {
        throw exception("couldn't insert INT");
    }
    if(argc>1)
    {
        std::cerr << "---> compiling" << std::endl;
        auto_ptr<Pattern> p = RegExp(argv[1],&dict);
        std::cerr << "---> save to binary" << std::endl;
        p->save_to("rx.bin");
        std::cerr << "---> save to graphViz" << std::endl;
        p->graphViz("rx.dot");
        std::cerr << "---> build expression" << std::endl;
        const string expr = p->toRegExp();
        std::cerr << "expr='" << expr << "'" << std::endl;
        std::cerr << "---> recompile expression" << std::endl;
        auto_ptr<Pattern> q = RegExp(expr,0);
        q->save_to("rx2.bin");
        q->graphViz("rx2.dot");
        Y_CHECK(*p==*q);
    }
    
}
Y_UTEST_DONE()

