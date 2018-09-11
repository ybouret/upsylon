#include "y/lang/pattern/posix.hpp"
#include "y/lang/pattern/dictionary.hpp"

#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ios/icstream.hpp"

using namespace upsylon;
using namespace Lang;

#define __CHECK(EXPR) do {                                          \
std::cerr << "Checking " << #EXPR << std::endl;                     \
std::cerr << "\t|_GraphViz" << std::endl;                           \
auto_ptr<Pattern> p = Posix::EXPR(); p->GraphViz( #EXPR ".dot" );   \
std::cerr << "\t|_saving" << std::endl;                             \
{ ios::ocstream fp( #EXPR ".bin" ); p->write(fp); }                 \
std::cerr << "\t|_reloading" << std::endl;                          \
{ ios::icstream fp( #EXPR ".bin");                                  \
auto_ptr<Pattern> q = Pattern::Load(fp);                            \
const string p_bin = p->to_binary();                                \
const string q_bin = q->to_binary();                                \
Y_CHECK(p_bin==q_bin);                                              \
}                                                                   \
std::cerr << "\t|_b64=" << p->to_base64() << std::endl;             \
dict(#EXPR,p.yield());                                              \
std::cerr << std::endl;                                             \
} while(false)

Y_UTEST(posix)
{
    Dictionary dict;
    __CHECK(lower);
    __CHECK(upper);
    __CHECK(alpha);
    __CHECK(digit);
    __CHECK(alnum);
    __CHECK(space);
    __CHECK(blank);
    __CHECK(xdigit);
    __CHECK(word);
    __CHECK(endl);
    __CHECK(dot);
    __CHECK(punct);
}
Y_UTEST_DONE()

