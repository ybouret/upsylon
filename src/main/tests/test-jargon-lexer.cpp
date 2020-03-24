

#include "y/jargon/lexer.hpp"
#include "y/jargon/lexeme.hpp"

#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;
using namespace Jargon;

namespace {
    
    
    class myLex : public Lexer
    {
    public:
        myLex() : Lexer( "myLex" )
        {
        }
        
        virtual ~myLex() throw()
        {
        }
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(myLex);
    };
}

Y_UTEST(jargon_lexer)
{
    myLex L;
    
}
Y_UTEST_DONE()

