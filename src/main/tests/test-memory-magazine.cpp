
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

Y_UTEST(magazine)
{
    //dummy::repo &mgr = dummy::repo::instance();

}
Y_UTEST_DONE()


