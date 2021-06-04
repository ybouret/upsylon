
#include "y/jive/syntax/axiom/all.hpp"

#include "y/jive/syntax/grammar.hpp"
#include "y/jive/common-regexp.hpp"
#include "y/jive/lexical/plugin/comments.hpp"
#include "y/jive/lexical/plugin/strings.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;
using namespace Jive;

namespace {

    class MyLexer : public Lexer
    {
    public:
        explicit MyLexer() : Lexer("MyLexer")
        {
            emit("ID",  RegExpFor::Identifier);
            emit("INT", RegExpFor::Integer);
            emit("XML", "<[:alpha:]+>");
            call( plug<CppComments>("cppComments") );
            drop_endl("endl","[:endl:]");
            drop("blank","[:blank:]");
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(MyLexer);
    };

    class MyGrammar : public Syntax::Grammar
    {
    public:
        explicit MyGrammar() : Syntax::Grammar("MyGrammar")
        {
            const Syntax::Axiom &ID   = term("ID", Syntax::Terminal::Standard);
            const Syntax::Axiom &INT  = term("INT",Syntax::Terminal::Standard);
            const Syntax::Axiom &XML  = term("XML",Syntax::Terminal::Standard);
            Syntax::Compound    &ITEM = agg("ITEM");
            ITEM << ID << option(INT);
            const Syntax::Axiom &REP = repeat( choice(ITEM,option(XML)),0);
            
            setRoot(REP);
            
            graphViz("grammar.dot");

            validate(NULL);
        }

        virtual ~MyGrammar() throw()
        {
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(MyGrammar);
    };

}

Y_UTEST(grammar)
{

    Syntax::Axiom::Verbose = true;
    {
        Syntax::Grammar   G("G");
        Syntax::Terminal &term = G.add( new Syntax::Terminal("term",Syntax::Terminal::Standard) );
        Syntax::Internal &dull = G.add( new Syntax::Dull("dull") );

        XNode::Pointer    root( XNode::Acquire(dull) );
        root->leaves().push_back( XNode::Acquire(term,NULL) );
        root->leaves().push_back( XNode::Acquire(dull) );

        root->save_to("dull.bin");
        size_t maxLength = 0;
        {
            Source         temp( Module::OpenFile("dull.bin") );
            XNode::Pointer load( XNode::Load(temp,G,maxLength) );
            Y_CHECK(load.is_valid());
        }
    }

    {
        MyGrammar        G;
        MyLexer          L;
        Syntax::Dull     dull("empty");
        

        if(argc>1)
        {
            Syntax::Axiom::Verbose = true;
            Source source( Module::OpenFile(argv[1]) );
            XNode::Pointer xnode( G.run(L,source) );
            if(xnode.is_valid())
            {
                std::cerr << "accepted " << xnode->axiom.name << std::endl;
            }
            else
            {
                std::cerr << "accepted empty tree" << std::endl;
                xnode = XNode::Acquire(dull);
            }
            xnode->graphViz("xnode.dot");
        }


    }


}
Y_UTEST_DONE()

