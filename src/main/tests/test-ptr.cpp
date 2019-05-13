#include "y/ptr/embedded.hpp"
#include "y/ptr/counted.hpp"
#include "y/ptr/arc.hpp"
#include "y/ptr/zrc.hpp"
#include "y/ptr/shared.hpp"
#include "y/ptr/auto.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace
{

    class derived : public counted_object
    {
    public:
        const int a;

        explicit derived( const int arg=0 ) : a(arg) {}
        virtual ~derived() throw() {}


    private:
        Y_DISABLE_COPY_AND_ASSIGN(derived);
    };

}

#define Y_CHECK_EMBED(PTR) \
embedded<derived,counted_object,PTR> emb0,emb1 = new derived(1);\
Y_CHECK(0==emb0->a);\
Y_CHECK(1==emb1->a)

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

        Y_CHECK_EMBED(arc_ptr);
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
        Y_CHECK_EMBED(shared_ptr);
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
        Y_CHECK_EMBED(auto_ptr);

    }
    
    {
        std::cerr << "-- ZRC" << std::endl;
        typedef zrc_ptr<counted> ZP;
        ZP p = NULL;
        Y_CHECK(p.is_empty()); std::cerr << "#p=" << p.refcount() << std::endl;
        auto_ptr<counted> tmp = new counted; std::cerr << "#counted=" << tmp->refcount() << std::endl;
        p    = tmp.yield();
        Y_CHECK(p.is_valid()); std::cerr << "#p=" << p.refcount() << std::endl;
        ZP q = p;
        Y_CHECK(q.is_valid());
        Y_CHECK( & *q == & * q);
        Y_CHECK_EMBED(zrc_ptr);

    }
    
    
}
Y_UTEST_DONE()
