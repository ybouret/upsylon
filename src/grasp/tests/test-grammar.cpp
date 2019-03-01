#include "y/lang/syntax/grammar.hpp"

#include "y/lang/syntax/terminal.hpp"
#include "y/lang/syntax/joker.hpp"
#include "y/lang/lexical/plugin/strings.hpp"

#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Lang;

class myLexer : public Lexer
{
public:
    explicit myLexer() : Lexer("myLexer")
    {
        Lexical::Scanner &root = **this;
        root.verbose = true;
        root.emit("ID","[:alpha:]+");
        // root.emit("INT","[:digit:]+");
        hook<Lexical::jString>(root,"STRING");


        root.drop("blanks","[:blank:]+");
        root.endl("endl", "[:endl:]");
    }

    virtual ~myLexer() throw()
    {
    }

private:
    Y_DISABLE_COPY_AND_ASSIGN(myLexer);
};

Y_UTEST(grammar)
{
    const Tag id = new string("grammar");

    Syntax::Grammar G( id );

    Syntax::Terminal   &ID      = G.decl( new Syntax::Terminal("ID") );
    Syntax::Rule       &STRING = G.decl( new Syntax::Terminal("STRING") );
    const Syntax::Rule &TOKEN  = G.choice(ID,STRING);
    Syntax::OneOrMore  &TOKENS = G.decl( new Syntax::OneOrMore( "TOKENS", TOKEN ) );
    std::cerr << "topLevel=" << G.topLevel().name << std::endl;
    G.topLevel(TOKENS);
    std::cerr << "topLevel=" << G.topLevel().name << std::endl;

    myLexer lexer;
    G.setVerbose(true);
    
    if(argc>1 && 0==strcmp(argv[1],"run"))
    {
        Source   source( Module::OpenSTDIN() );
#if 0
        Lexeme                 *lx  = 0;
        while( NULL != (lx=lexer.get(source) ) )
        {
            auto_ptr<Lexeme> guard(lx);
            std::cerr << "+" << *lx << "@" << lx->line() << ":" << lx->column() << std::endl;
        }
#endif
        auto_ptr<Syntax::Node> cst = G.accept(source,lexer);
        std::cerr << "parsed..." << std::endl;
        cst->graphViz("cst.dot");
    }


}
Y_UTEST_DONE()

