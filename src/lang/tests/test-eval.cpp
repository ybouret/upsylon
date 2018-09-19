
#include "y/lang/compiler.hpp"
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
        Eval() : Syntax::Parser("evaluator")
        {
        }

        virtual ~Eval() throw()
        {
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Eval);
    };

    class Evaluator : public Compiler
    {
    public:
        Evaluator() :  Compiler( new Eval()  )
        {
        }

        virtual ~Evaluator() throw()
        {
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Evaluator);
    };

}

Y_UTEST(eval)
{
    Evaluator ev;

    if(argc>1)
    {
        const string filename = argv[1];
        if("NULL"==filename) return 0;
        ev.compile( Module::OpenFile(filename) );
    }
    else
    {
        ev.compile( Module::OpenSTDIN() );
    }
}
Y_UTEST_DONE()




