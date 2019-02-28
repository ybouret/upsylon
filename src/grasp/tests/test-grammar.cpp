#include "y/lang/syntax/grammar.hpp"

#include "y/lang/syntax/terminal.hpp"
#include "y/lang/syntax/joker.hpp"

#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Lang;

Y_UTEST(grammar)
{
    const Tag id = new string("grammar");

    Syntax::Grammar G( id );

    Syntax::RuleReferenceSet rrs;
    std::cerr << "rrs.capacity  = " << rrs.capacity() << std::endl;
    std::cerr << "rss.allocated = " << rrs.allocated_bytes_for_table() << std::endl;
}
Y_UTEST_DONE()

