
#include "y/jive/pattern/posix.hpp"
#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"

using namespace upsylon;
using namespace Jive;

namespace {

    static inline
    void do_check( const Motif &p, const string &id )
    {
        const string binName = id + ".dat";
        p->save_to(binName);
        const string dotName = id + ".dot";
        p->graphViz(dotName);
    }

}

#define CHECK(NAME) do { const string id = #NAME; \
std::cerr << "<posix::" << id << ">" << std::endl;\
const Motif p = posix::NAME(); do_check(p,id); \
std::cerr << "<posix::" << id << "/>" << std::endl << std::endl;;\
} while(false)

Y_UTEST(jive_posix)
{
    CHECK(lower);
    CHECK(upper);
    CHECK(alpha);
    CHECK(digit);
    CHECK(alnum);
    CHECK(xdigit);
    CHECK(blank);
    CHECK(space);
    CHECK(punct);


}
Y_UTEST_DONE()
