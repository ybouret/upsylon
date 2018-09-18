
#include "y/lang/syntax/parser.hpp"
#include "y/lang/syntax/analyzer.hpp"
#include "y/lang/lexical/plugin/comment.hpp"
#include "y/lang/lexical/plugin/strings.hpp"

#include "y/utest/run.hpp"
#include "y/fs/local.hpp"

using namespace upsylon;
using namespace Lang;

namespace
{
    class Eval : public Syntax::Parser
    {
    public:

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Eval);
    };

}

Y_UTEST(eval)
{

}
Y_UTEST_DONE()




