#include "y/lang/syntax/parser.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Lang;

class myParser : public Syntax::Parser
{
public:
    inline myParser() : Syntax::Parser("JSON")
    {
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


