
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



    const Origin    gID = new string("grammar");
    myLex           lexer;
    Syntax::Grammar G(gID);

    Syntax::Rule &ID = G.terminal("ID");
    (void)ID;

    if(argc>1&&0==strcmp(argv[1],"run"))
    {
        std::cerr << "running..." << std::endl;
        {
            Source source( Module::OpenSTDIN() );
            auto_ptr<Syntax::Node> Tree = G.run(lexer,source);
        }
        std::cerr << "...done" << std::endl;
    }


    

}
Y_UTEST_DONE()

