
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
            (void) emit("ID", "[:word:]+");
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
            const Language::Axiom &ID = term("ID", Language::Terminal::Standard);
        }

        virtual ~MyGrammar() throw()
        {
        }

    private:
        Y_DISABLE_ASSIGN(MyGrammar);
    };

}

Y_UTEST(lang_grammar)
{
    Language::Axiom::Verbose = true;

    {
        Language::Grammar G("G");

        const Language::Terminal &ID  = G.term("ID", Language::Terminal::Standard);
        const Language::Axiom    &INT = G.term("INT",Language::Terminal::Standard);
        Y_CHECK(G.query("ID"));
        Y_CHECK(G.query("INT"));
        G.setRoot(ID);  Y_CHECK(&ID==G.getRoot());
        G.setRoot(INT); Y_CHECK(&INT==G.getRoot());
    }

    MyLexer   lexer;
    MyGrammar grammar;
    if(argc>1)
    {
        Source source( Module::OpenFile(argv[1]));
        xTree  tree( grammar.run(source,lexer) );

    }

}
Y_UTEST_DONE()

