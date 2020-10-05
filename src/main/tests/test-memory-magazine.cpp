
#include "y/memory/magazine.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace {

    class dummy
    {
    public:
        int a;
        inline   dummy() throw() : a(0) {}
        inline   dummy(int x)    : a(x) { if(0==x) throw exception("dummy(0)"); }
        inline  ~dummy() throw() {}

        typedef memory::magazine<dummy> repo;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(dummy);
    };
}

namespace upsylon
{
    namespace memory
    {
        Y_SINGLETON_TEMPLATE_WITH(0,memory::magazine<dummy>,dummy:repo);
    }
}


Y_UTEST(magazine)
{
    concurrent::singleton::verbose = true;

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

}
Y_UTEST_DONE()


