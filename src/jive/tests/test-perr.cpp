

#include "y/jive/parser.hpp"
#include "y/utest/run.hpp"
#include "y/jive/syntax/analyzer.hpp"

using namespace upsylon;
using namespace Jive;

namespace
{

    class P : public Parser
    {
    public:
        inline P() : Parser( "P" )
        {

            terminal("ID", "[:word:]+");

            // lexical only
            drop("blank","[:blank:]");
            endl("endl", "[:endl:]");
            graphViz("perr.dot");
            validate();
        }

        inline virtual ~P() throw()
        {
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(P);
    };

}

Y_UTEST(perr)
{
    P parser;
    if(argc>1)
    {
        XNode::Pointer ast( parser.parseFile(argv[1]) );
    }
}
Y_UTEST_DONE()

