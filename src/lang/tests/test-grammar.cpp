
#include "y/lang/syntax/terminal.hpp"
#include "y/lang/syntax/joker.hpp"

#include "y/lang/syntax/grammar.hpp"
#include "y/lang/lexical/plugin/comment.hpp"

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
            root.emit("EQ",  "=");
            root.endl("endl",  "[:endl:]");
            root.drop("blanks","[:blank:]+");
            hook<Lexical::CXX_Comment>(root,"CXX_Comment");
        }

        virtual ~myLex() throw()
        {

        }



    private:
        Y_DISABLE_COPY_AND_ASSIGN(myLex);

    };
}

#include "y/fs/local.hpp"

Y_UTEST(grammar)
{
    SHOW(Syntax::Node);
    SHOW(Syntax::Node::List);
    SHOW(Syntax::Rule);
    SHOW(Syntax::Rule::List);

    vfs &fs = local_fs::instance();
    fs.try_remove_file("tree.dot");
    fs.try_remove_file("tree.png");

    const Origin    gID = new string("grammar");
    myLex           lexer;
    Syntax::Grammar G(gID);

    Syntax::Rule &ID  = G.terminal("ID");
    Syntax::Rule &INT = G.terminal("INT");
    Syntax::Rule &END = G.terminal("END",Syntax::Terminal::Semantic);
    Syntax::Rule &EQ  = G.terminal("EQ", Syntax::Terminal::Univocal);

    Syntax::Aggregate &show = G.agg("show");
    show << G.choice(ID,INT) << END;

    Syntax::Aggregate &assign = G.agg("assign");
    assign << ID << EQ << G.choice(ID,INT) << END;

    Syntax::Aggregate &code = G.agg("code");
    code << G.zeroOrMore( G.choice(assign,show) );



    G.top( code );

    G.GraphViz("gram.dot");
    G.checkValidity();
    

    if(argc>1&&0==strcmp(argv[1],"run"))
    {
        std::cerr << "running..." << std::endl;
        {
            Source source( Module::OpenSTDIN() );
            auto_ptr<Syntax::Node> Tree = G.run(lexer,source);
            assert(Tree.is_valid());
            Tree->GraphViz("tree.dot");
            {
                Syntax::Node *tree = Tree.yield();
                Tree = Syntax::Node::AST(tree);
            }
            Tree->GraphViz("ast.dot");
        }
        std::cerr << "...done" << std::endl;
    }


    

}
Y_UTEST_DONE()

