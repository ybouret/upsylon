#include "y/ptr/counted.hpp"
#include "y/ptr/arc.hpp"
#include "y/ptr/shared.hpp"
#include "y/ptr/auto.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace
{
    
}

Y_UTEST(ptr)
{
    {
        typedef arc_ptr<counted> ARC;
        ARC p( new counted_object() );
        Y_CHECK(p.is_valid());
        Y_CHECK( 1==p->refcount() );
        ARC q = p;
        Y_CHECK( 2==p->refcount() );
        Y_CHECK( 2==q->refcount() );
        ARC r = new counted_object();
        r = p;
        Y_CHECK( 3==r->refcount() );
    }
    
    {
        typedef shared_ptr<counted> SHP;
        SHP p( new counted_object() );
        Y_CHECK( p.is_valid() );
        Y_CHECK( 1==p.refcount() );
        SHP q = p;
        Y_CHECK( 2==p.refcount() );
        Y_CHECK( 2==q.refcount() );
        SHP r = new counted_object();
        r = p;
        Y_CHECK( 3==r.refcount() );
    }
    
    
    
}
Y_UTEST_DONE()
