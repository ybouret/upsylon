
#include "y/jive/pattern/all.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"

using namespace upsylon;
using namespace Jive;

Y_UTEST(jive_opt)
{
   
    
    auto_ptr<Pattern> q = 0;
    {
        Logical *p = And::Create();
        q          = Optional::Create(p);
        
        p->add('a');
        {
            Logical *r = And::Create();
            p->push_back(r);
            r->add('b');
            r->add('c');
        }
        p->add('d');
        {
            Logical *r = Or::Create();
            p->push_back(r);
            r->add('e');
        }
        
        
    }
    q->graphViz("ini.dot");
    q = Pattern::Optimize( q.yield() );
    q->graphViz("opt.dot");

    
}
Y_UTEST_DONE()

