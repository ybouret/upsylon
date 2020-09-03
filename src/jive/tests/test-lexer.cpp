
#include "y/utest/run.hpp"
#include "y/jive/lexer.hpp"

using namespace upsylon;
using namespace Jive;

namespace {

    class MyLexer : public Lexer
    {
    public:
        explicit MyLexer() : Lexer( "Lexer" )
        {
        }


        virtual ~MyLexer() throw()
        {
        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(MyLexer);
    };
}

Y_UTEST(lexer)
{
    MyLexer lexer;
}
Y_UTEST_DONE()
