#include "y/ptr/counted.hpp"
#include "y/ptr/arc.hpp"
#include "y/ptr/zrc.hpp"
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
        std::cerr << "-- ARC" << std::endl;
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
        std::cerr << "-- SHARED" << std::endl;
        typedef shared_ptr<counted> SHP;
        SHP p = new counted_object();
        Y_CHECK( p.is_valid() );
        Y_CHECK( 1==p.refcount() );
        SHP q = p;
        Y_CHECK( 2==p.refcount() );
        Y_CHECK( 2==q.refcount() );
        SHP r = new counted_object();
        r = p;
        Y_CHECK( 3==r.refcount() );
    }

    {
        std::cerr << "-- AUTO" << std::endl;
        typedef auto_ptr<counted> AP;
        AP p = new counted_object();
        Y_CHECK(p.is_valid());

        AP q = p;
        Y_CHECK(q.is_valid());
        Y_CHECK(!p.is_valid());
        q = NULL;
        Y_CHECK(!q.is_valid());
        p = new counted_object();
        Y_CHECK(p.is_valid());
        q = p;
        Y_CHECK(!p.is_valid());
        Y_CHECK(q.is_valid());
        
    }
    
    {
        std::cerr << "-- ZRC" << std::endl;
        typedef zrc_ptr<counted> ZP;
        ZP p = NULL;
        Y_CHECK(p.is_empty());
        p    = new counted();
        Y_CHECK(p.is_valid());
        ZP q = p;
        Y_CHECK(q.is_valid());
        Y_CHECK( & *q == & * q);

    }
    
    
}
Y_UTEST_DONE()
