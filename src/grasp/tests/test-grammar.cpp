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
        //root.emit("INT","[:digit:]+");
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

    Syntax::Terminal   &ID     = G.decl( new Syntax::Terminal("ID")     );
    Syntax::Rule       &STRING = G.decl( new Syntax::Terminal("STRING") );
    const Syntax::Rule &TOKEN  = G.choice(ID,STRING);
    const Syntax::Rule &TOKENS = G.oneOrMore(TOKEN);
    std::cerr << "topLevel=" << G.topLevel().name << std::endl;
    G.topLevel(TOKENS);
    std::cerr << "topLevel=" << G.topLevel().name << std::endl;

    myLexer lexer;
    G.setVerbose(true);
    G.graphViz("grammar.dot");
    G.finalize();



    if(argc>1 && 0==strcmp(argv[1],"run"))
    {
        string s1;
        {
            Source                 source( Module::OpenSTDIN() );
            auto_ptr<Syntax::Node> cst = G.accept(source,lexer);
            std::cerr << "parsed..." << std::endl;
            cst->graphViz("cst.dot");
            cst->save("cst.bin");
            s1 = cst->toBase64();
        }
        std::cerr << "s1=" << s1 << std::endl;
        string s2;
        {
            Source source( Module::OpenFile("cst.bin") );
            auto_ptr<Syntax::Node> cst = Syntax::Node::Load(source,G);
            cst->graphViz("cst2.dot");
            s2 = cst->toBase64();
        }
        std::cerr << "s2=" << s2 << std::endl;
        Y_CHECK(s1==s2);

    }


}
Y_UTEST_DONE()

