#include "y/lang/syntax/parser.hpp"
#include "y/lang/lexical/plugin/comment.hpp"

#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Lang;

namespace
{
    class myParser : public Syntax::Parser
    {
    public:
        explicit myParser() : Syntax::Parser("JSON")
        {
            hook<Lexical::CXX_Comment>("C++ Comment");

            
        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(myParser);
    };
}

Y_UTEST(parser)
{
    myParser json;
    
}
Y_UTEST_DONE()


