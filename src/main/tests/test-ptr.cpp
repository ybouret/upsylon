#include "y/ptr/counted.hpp"
#include "y/ptr/arc.hpp"

#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(ptr)
{
    typedef arc_ptr<counted> ARC;
    ARC p( new counted_object() );
    Y_CHECK( p.__get() );
    Y_CHECK( 1==p->refcount() );
    ARC q = p;
    Y_CHECK( 2==p->refcount() );
    Y_CHECK( 2==q->refcount() );
    
}
Y_UTEST_DONE()
