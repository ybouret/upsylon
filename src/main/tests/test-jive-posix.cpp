
#include "y/jive/pattern/posix.hpp"
#include "y/jive/pattern/dictionary.hpp"

#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"

using namespace upsylon;
using namespace Jive;

namespace {

    static inline
    void do_check( const Motif &p, const string &id, Dictionary &dict)
    {
        Y_CHECK(posix::exists(id));
        const string binName = id + ".dat";
        p->save_to(binName);
        const Motif q = posix::create(id);
        Y_CHECK(*p==*q);
        const string dotName = id + ".dot";
        p->graphViz(dotName);
        FirstChars fc;
        p->start(fc);
        std::cerr << "first chars: " << fc << std::endl;
        if( !dict.insert_by(id,p) )
        {
            throw exception("multiple <%s> in dict", *id);
        }
    }

}

#define CHECK(NAME) do { const string id = #NAME;   \
std::cerr << "<posix::" << id << ">" << std::endl;  \
const Motif p = posix::NAME(); do_check(p,id,dict); \
std::cerr << "<posix::" << id << "/>" << std::endl << std::endl;;\
} while(false)

Y_UTEST(jive_posix)
{
    Dictionary dict;
    posix::root().graphViz("posix.dot");

    CHECK(lower);
    CHECK(upper);
    CHECK(alpha);
    CHECK(digit);
    CHECK(alnum);
    CHECK(xdigit);
    CHECK(blank);
    CHECK(space);
    CHECK(punct);

    CHECK(word);
    CHECK(endl);
    CHECK(dot);
    CHECK(core);

    dict.get_root().graphViz("dict.dot");
}
Y_UTEST_DONE()
