
#include "y/lang/syntax/terminal.hpp"
#include "y/lang/syntax/joker.hpp"

#include "y/lang/syntax/grammar.hpp"

#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace Lang;

#define SHOW(CLASS) std::cerr << "sizeof(" #CLASS << ")=" << sizeof(CLASS) << std::endl

namespace
{
    class myLex : public Lexer
    {
    public:
        explicit myLex() : Lexer("lexer")
        {
            Scanner &root = **this;
            root.emit("ID", "[:alpha:]+");
            root.emit("INT", "[:digit:]+");
            root.emit("END", ";");
            root.emit("ADDOP", "[-+]");
            root.emit("MULOP", "[*/]");
            root.endl("endl",  "[:endl:]");
            root.drop("blanks","[:blank:]+");
        }

        virtual ~myLex() throw()
        {

        }



    private:
        Y_DISABLE_COPY_AND_ASSIGN(myLex);

    };
}

Y_UTEST(grammar)
{
    SHOW(Syntax::Node);
    SHOW(Syntax::Node::List);
    SHOW(Syntax::Rule);
    SHOW(Syntax::Rule::List);

#if 0
    {
        const Origin label = new string("id");
        const Syntax::Terminal ID("id");
        {
            auto_ptr<Syntax::Node> node( Syntax::Node::Create(ID) );
            Lexeme *lx = new Lexeme(label);
            node->children.push_back( Syntax::Node::Create(ID,lx) );
            node->children.push_back( Syntax::Node::Create(ID) );
            Y_CHECK(node->internal);
            node->GraphViz("internal.dot");
        }

        {
            Lexeme *lx = new Lexeme(label);
            auto_ptr<Syntax::Node> node( Syntax::Node::Create(ID,lx) );
            Y_CHECK(node->terminal);
            node->GraphViz("terminal.dot");
        }
        {
            Lexeme *lx = new Lexeme(label);
            auto_ptr<Syntax::Node> node( Syntax::Node::Create(ID,lx) );
            Y_CHECK(node->terminal);
            delete Syntax::Node::Yield( node.yield() );
        }
    }
#endif

    const Origin    gID = new string("grammar");
    myLex           lexer;
    Syntax::Grammar G(gID);

    G.add( new Syntax::Terminal("ID") );

    if(argc>1&&0==strcmp(argv[1],"run"))
    {
        std::cerr << "running..." << std::endl;
        {
            Source source( Module::OpenSTDIN() );
            while(true)
            {
                Lexeme *lx = lexer.get(source);
                if(!lx)
                {
                    break;
                }
                std::cerr << lx->label << " : '" << *lx << "'@" << lx->line() << std::endl;
                delete lx;
            }
        }
        std::cerr << "...done" << std::endl;
    }


    

}
Y_UTEST_DONE()

