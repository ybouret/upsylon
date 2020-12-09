
#include "y/memory/magazine.hpp"
#include "y/utest/run.hpp"
#include "y/core/inode.hpp"

using namespace upsylon;

namespace {

    class simple
    {
    public:
        static int count;
        inline  simple() throw() { ++count; }
        inline ~simple() throw() { --count; }
        inline  simple(const simple &) throw() { ++count; }

        typedef memory::magazine<simple> repo;

    private:
        Y_DISABLE_ASSIGN(simple);
    };

    int simple::count = 0;


    class dummy : public inode<dummy>
    {
    public:
        int      a;
        inline   dummy() throw() : inode<dummy>(), a(0) {}
        inline   dummy(int x)    : a(x) { if(0==x) throw exception("dummy(0)"); }
        inline   dummy(const dummy &other) throw() : inode<dummy>(), a(other.a) {}
        inline  virtual ~dummy() throw() {}

        typedef memory::magazine<dummy> repo;

        typedef repo::list_ list;

    private:
        Y_DISABLE_ASSIGN(dummy);
    };


}

namespace upsylon
{
    namespace memory
    {
        Y_SINGLETON_TEMPLATE_WITH(7,simple::repo);
        Y_SINGLETON_TEMPLATE_WITH(0,dummy::repo);
        
    }
}


Y_UTEST(magazine)
{
    concurrent::singleton::verbose = true;

    {
        simple::repo &mgr = simple::repo::instance();
        
        std::cerr << "using " << mgr.call_sign << std::endl;
        
        simple *s = mgr.acquire();
        simple *c = mgr.copycat(s);
        mgr.release(s);
        mgr.release(c);

        {
            simple::repo::auto_ptr keep( mgr.acquire() );
            Y_CHECK(keep.is_valid());
        }

        {
            simple::repo::auto_ptr keep( mgr.acquire() );
            mgr.release( keep.yield() );
            Y_CHECK(keep.is_empty());
            keep = mgr.acquire();
            keep = mgr.acquire();
        }

        Y_CHECK(simple::count==0);


    }

    {
        dummy::repo &mgr = dummy::repo::instance();
        std::cerr << "using " << mgr.call_sign << std::endl;

        {
            dummy *d = mgr.acquire();
            std::cerr << "a=" << d->a << std::endl;
            mgr.release(d);
        }

        {
            dummy *d = mgr.acquire<int>(5);
            std::cerr << "a=" << d->a << std::endl;
            mgr.release(d);
        }

        try
        {
            dummy *d = mgr.acquire<int>(0);
            mgr.release(d);
        }
        catch(const exception &e)
        {
            std::cerr << "OK: " << e.when() << std::endl;
        }

        {
            dummy::list L;
            for(size_t i=1000;i>0;--i)
            {
                if( alea.choice() )
                {
                    L.push_back( mgr.acquire<int>( alea.full<int>() ) );
                }
                else
                {
                    L.push_front( mgr.acquire<int>( alea.full<int>() ) );
                }
            }
            dummy::list L2(L);

            L.push( mgr.acquire() );
        }

        std::cerr << "#prefetched: " << mgr.prefetched() << std::endl;
    }
}
Y_UTEST_DONE()


