#include "y/lang/token.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Lang;

namespace
{
    static inline void display( const Token &t )
    {
        std::cerr << "-- #chars=" << t.size << std::endl;
        std::cerr << t << "|" << t.to_string() << "|" << t.to_visible_string() << std::endl;
    }
}

Y_UTEST(token)
{
    {
        const string s = "\tHello, World!\"\\\r\n";
        Token t;        display(t);
        Token t1 = s;   display(t1);
        Token t2 = *s;  display(t2);
        t = t1;         display(t);

    }

    std::cerr << "cache.size=" << Manager::instance().size << std::endl;
}
Y_UTEST_DONE()


