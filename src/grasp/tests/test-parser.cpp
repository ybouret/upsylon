#include "y/lang/syntax/parser.hpp"
#include "y/utest/run.hpp"
#include "y/lang/lexical/plugin/strings.hpp"

using namespace upsylon;
using namespace Lang;

class myParser : public Syntax::Parser
{
public:
    inline myParser() : Syntax::Parser("JSONLite")
    {
        TERM &ID    = term("ID","[:alpha:]+");
        TERM &COLON  = term(":",':');
        TERM &COMA   = term(',').setSemantic();
        TERM &PLUS   = term("PLUS",'+').setOperator();
        RULE &STRING = hook<Lexical::jString>("STRING");


        // lexical only
        (**this).drop("[:blank:]+");
        (**this).endl("[:endl:]");

        graphViz("jsonlite.dot");
    }

    inline virtual ~myParser() throw()
    {
    }

private:
    Y_DISABLE_COPY_AND_ASSIGN(myParser);
};

Y_UTEST(parser)
{
    Syntax::Parser::Pointer P = new myParser();
    std::cerr << "Building [" << P->key() << "]" << std::endl;
}
Y_UTEST_DONE()


