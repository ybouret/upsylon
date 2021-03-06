#include "y/jive/pattern/basic/rework.hpp"
#include "y/jive/pattern/all.hpp"

#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"

using namespace upsylon;
using namespace Jive;

Y_UTEST(jive_rework)
{

    {
        auto_ptr<Or> p = Or::Create();
        auto_ptr<Or> q = Or::Create();

        p->push_back( Single::Create('a') );
        p->push_back( Single::Create('a') );
        p->push_back( Single::Create('b') );
        p->push_back( Single::Create('c') );
        p->push_back( Range::Create('e','k') );
        p->push_back( Single::Create('h') );
        p->push_back( And::Create() );
        p->push_back( Single::Create('l') );

        p->push_back( Range::Create('q','z') );
        p->push_back( Range::Create('q','z') );
        p->push_back( Range::Create('r','w') );

        p->push_back( Range::Create('A','F') );
        p->push_back( Range::Create('D','K') );

        p->push_back( Range::Create('N','R') );
        p->push_back( Range::Create('S','X') );



        q->merge_back_copy(*p);

        p->graphViz("raw.dot");
        p->save_to("raw.bin");

        p->rework();
        p->graphViz("opt.dot");
        p->save_to("opt.bin");

        alea.shuffle( static_cast<core::list_of<Pattern>& >(*q) );
        q->rework();
        q->graphViz("bis.dot");
    }
    
    {
        auto_ptr<Or> p = Or::Create();
        p->add('a','d');
        p->add('e');
        p->push_back( Optional::Create( Single::Create('f')) );
        p->add('z');
        p->add('k');
        p->push_back( Repeating::Create( Single::Create('p'), 0));
        p->add('y');
        
        p->graphViz("raw2.dot");
        p->save_to("raw2.bin");
        
        p->rework();
        p->graphViz("opt2.dot");
        p->save_to("opt2.bin");
    }

}
Y_UTEST_DONE()

