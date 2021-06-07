
#include "y/jive/language/grammar.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;
using namespace Jive;

namespace
{

    class MyLexer : public Lexer
    {
    public:
        explicit MyLexer() : Lexer("MyLexer")
        {
            (void) emit("ID", "[:alpha:]+");
            (void) emit("INT", "[:digit:]+" );
            drop_endl("[:endl:]", "[:endl:]");
            drop("[:blank:]", "[:blank:]");
        }

        virtual ~MyLexer() throw()
        {
        }

    };

    class MyGrammar : public Language::Grammar
    {
    public:
        explicit MyGrammar() : Language::Grammar("MyGrammar")
        {

            const Axiom &ID  = term("ID",Language::Terminal::Standard);
            const Axiom &INT = term("INT",Language::Terminal::Standard);
            const Axiom &WHO = (agg("WHO") << ID <<  opt(INT) );
            setRoot(WHO);

            {
                const Axiom &REP = rep(WHO,0);
                setRoot(REP);
            }
        }

        virtual ~MyGrammar() throw()
        {
        }

    private:
        Y_DISABLE_ASSIGN(MyGrammar);
    };

}
#include "y/fs/local/fs.hpp"

Y_UTEST(grammar)
{
    Language::Axiom::Verbose = true;
    vfs &fs = local_fs::instance();

    fs.try_remove_file("tree.dot");
    fs.try_remove_file("tree.png");


    {
        Language::Grammar         G("G");
        const Language::Terminal &ID  = G.term("ID", Language::Terminal::Standard);
        const Language::Axiom    &INT = G.term("INT",Language::Terminal::Standard);
        Y_CHECK(G.query("ID"));
        Y_CHECK(G.query("INT"));
        G.setRoot(ID);  Y_CHECK(&ID==G.getRoot());
        G.setRoot(INT); Y_CHECK(&INT==G.getRoot());
    }

    MyLexer   lexer;
    MyGrammar grammar;
    grammar.validateWith(&lexer);
    
    grammar.graphViz("grammar.dot");
    
    if(argc>1)
    {
        Source source( Module::OpenFile(argv[1]));
        XTree  tree( grammar.run(source,lexer) );
        if( tree.is_valid() )
        {
            tree->graphViz("tree.dot");
        }
    }

}
Y_UTEST_DONE()

