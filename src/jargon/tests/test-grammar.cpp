#include "y/jargon/grammar.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/jargon/lexical/plugin/end-of-line-comment.hpp"
#include "y/jargon/axiom/visitor.hpp"
#include "y/jargon/evaluator.hpp"

using namespace  upsylon;
using namespace  Jargon;


namespace {
    
    class myLexer   : public Lexer
    {
    public:
        explicit myLexer() :  Lexer("sample")
        {
            
            emit("ID",    "[:alpha:]+");
            emit("INT",   "[:digit:]+");
            emit("SEP",   ';');
            endl("endl",  "[:endl:]");
            drop("blanks","[:blank:]");
            load(type2type<Lexical::ShellComment>(),"comment").hook( *this );
        }
        
        virtual ~myLexer() throw()
        {
        }
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(myLexer);
    };
    
    class myGrammar : public Grammar
    {
    public:
        virtual ~myGrammar() throw()
        {
            
        }
        
        myGrammar() : Grammar("myGrammar")
        {
            
            Axiom    &ID   = _terminal("ID",  Terminal::Standard);
            Axiom    &INT  = _terminal("INT", Terminal::Standard);
            Compound &INFO = agg("INFO");
            INFO << ID << opt(choice(INT,ID)) << _terminal("SEP",Terminal::Division);
            Axiom &ROOT = zom(INFO);
            setGround(ROOT);
            displayAxioms();
        }
        
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(myGrammar);
    };
    
}
Y_UTEST(grammar)
{
    
    Y_UTEST_SIZEOF(Axiom);
    Y_UTEST_SIZEOF(Axioms);
    std::cerr << std::endl;
    
    Y_UTEST_SIZEOF(Terminal);
    Y_UTEST_SIZEOF(Internal);
    Y_UTEST_SIZEOF(Operator);
    std::cerr << std::endl;
    
    Y_UTEST_SIZEOF(Member);
    Y_UTEST_SIZEOF(Manifest);
    Y_UTEST_SIZEOF(Compound);
    Y_UTEST_SIZEOF(Aggregate);
    Y_UTEST_SIZEOF(Alternate);
    std::cerr << std::endl;
   
    Y_UTEST_SIZEOF(Option);
    Y_UTEST_SIZEOF(Repeat);
    Y_UTEST_SIZEOF(OneOrMore);
    Y_UTEST_SIZEOF(ZeroOrMore);

    
    
    {
        Grammar G("G");
        std::cerr << "grammar " << G.title  << std::endl;
        
        Terminal  &term = G.declare( new Terminal("word") );
        Aggregate &cmpd = G.declare( new Aggregate("hello") );
        
        cmpd << term;
        cmpd << G.opt(term);
        cmpd << G.oom(term);
        cmpd << G.zom(term);
        
        
        G.displayAxioms();
        std::cerr << "ground=<" << G.getGround().label << ">" << std::endl;
        Tags::Display();
        std::cerr << std::endl;
        Tags::Release();
        G.graphViz("G0.dot");

    }
    
    {
        Axiom::  Verbose = true;
        Lexical::Scanner::Verbose = true;

        myGrammar G;
        myLexer   L;
        Tags::Display();
        
        G.graphViz("G.dot");
        
        {
            Visitor v;
            v.supplyWith(G.getGround());
            v.display();
            std::cerr << "#axioms  = " << (*G).entries() << std::endl;
            std::cerr << "#visited = " << v.entries()    << std::endl;
        }
        
        G.validate();
        
        if(argc>1)
        {
            Source          source( Module::OpenFile(argv[1]) );
            auto_ptr<XNode> tree = G.accept(L,source);
            Y_CHECK(tree.is_valid());
            tree->graphViz("tree.dot");
            Evaluator_ E(G.title);
            E.browse(*tree);
            
            tree->save_to("tree.bin");
            
        }
    }
    
    
}
Y_UTEST_DONE()


