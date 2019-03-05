#include "y/lang/syntax/terminal.hpp"
#include "y/lang/syntax/joker.hpp"
#include "y/lang/syntax/compound.hpp"

#include "y/utest/run.hpp"

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
    std::cerr << "sizeof(Node)         = " << sizeof(Syntax::Node)         << std::endl;
    std::cerr << "sizeof(TerminalNode) = " << sizeof(Syntax::TerminalNode) << std::endl;
    std::cerr << "sizeof(InternalNode) = " << sizeof(Syntax::InternalNode) << std::endl;

}
Y_UTEST_DONE()


