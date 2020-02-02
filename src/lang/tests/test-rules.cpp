#include "y/lang/syntax/terminal.hpp"
#include "y/lang/syntax/joker.hpp"
#include "y/lang/syntax/compound.hpp"
#include "y/lang/syntax/rule-probe.hpp"

#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/lang/syntax/node/terminal.hpp"

using namespace upsylon;
using namespace Lang;

Y_UTEST(rules)
{
    Syntax::Rule::List rules;
    rules.push_back( new Syntax::Terminal("ID"));
    rules.push_back( new Syntax::Optional(   "OPT_ID", *rules.head ) );
    rules.push_back( new Syntax::ZeroOrMore( "ZOM_ID", *rules.head ) );
    rules.push_back( new Syntax::OneOrMore(  "OOM_ID", *rules.head ) );
    rules.push_back( new Syntax::Aggregate( "AGG" ) );
    rules.push_back( new Syntax::Alternate( "ALT" ) );

    for( const Syntax::Rule *r = rules.head; r; r=r->next )
    {
        std::cerr << "typeName=" << r->typeName() << " : " << r->name << std::endl;
    }

    __sizeof::width = 24;

    Y_UTEST_SIZEOF(Syntax::Node);
    Y_UTEST_SIZEOF(Syntax::TerminalNode);
    Y_UTEST_SIZEOF(Syntax::InternalNode);
    Y_UTEST_SIZEOF(Syntax::ExtendedNode);
    Y_UTEST_SIZEOF(Syntax::Node::Data);
    
    
}
Y_UTEST_DONE()


