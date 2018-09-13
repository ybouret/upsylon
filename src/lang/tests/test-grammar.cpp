
#include "y/lang/syntax/terminal.hpp"

#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace Lang;

#define SHOW(CLASS) std::cerr << "sizeof(" #CLASS << ")=" << sizeof(CLASS) << std::endl

Y_UTEST(grammar)
{
    SHOW(Syntax::Node);
    SHOW(Syntax::Node::List);
    SHOW(Syntax::Rule);
    SHOW(Syntax::Rule::List);

    const Origin label = new string("id");
    const Syntax::Terminal ID("id");
    {
        auto_ptr<Syntax::Node> node( Syntax::Node::Create(ID) );
        Y_CHECK(node->internal);
        Lexeme *lx = new Lexeme(label);
        node->children.push_back( Syntax::Node::Create(ID,lx) );
        node->children.push_back( Syntax::Node::Create(ID) );
    }

    Lexeme *lx = new Lexeme(label);
    {
        auto_ptr<Syntax::Node> node( Syntax::Node::Create(ID,lx) );
        Y_CHECK(node->terminal);
    }

}
Y_UTEST_DONE()

