#include "y/lang/syntax/terminal.hpp"
#include "y/lang/syntax/joker.hpp"

#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Lang;

Y_UTEST(rules)
{
    Syntax::Rule::List rules;
    rules.push_back( new Syntax::Terminal("ID"));
    rules.push_back( new Syntax::Optional(   "OPT_ID", *rules.head ) );
    rules.push_back( new Syntax::ZeroOrMore( "ZOM_ID", *rules.head ) );

    for( const Syntax::Rule *r = rules.head; r; r=r->next )
    {
        std::cerr << "typeName=" << r->typeName() << std::endl;
    }

}
Y_UTEST_DONE()


