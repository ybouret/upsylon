
#include "y/memory/magazine.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace {

    class dummy
    {
    public:
        inline   dummy() throw() {}
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

}
Y_UTEST_DONE()


