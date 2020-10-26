#include "y/jive/parser.hpp"
#include "y/utest/run.hpp"
#include "y/jive/lexical/plugin/single-line-comments.hpp"

using namespace upsylon;
using namespace Jive;

namespace
{
    class ListParser : public Parser
    {
    public:
        inline virtual ~ListParser() throw()
        {


        }

        inline explicit ListParser() : Parser("Lists")
        {
            Aggregate   &LIST     = agg("list");
            const Axiom &NAME     = terminal("NAME","[:word:]+");
            {
                Aggregate &ELEMENTS = grp("elements");
                LIST << division('[') << ELEMENTS << division(']');
                Compound &ELEMENT  = alt("element") << NAME << LIST;
                ELEMENTS << ELEMENT << zeroOrMore( cat( division(','), ELEMENT ) );
            }

            // lexical only
            plug<Lexical::SingleLineComments>("COMMENT","//");
            drop("blank","[:blank:]");
            endl("endl","[:endl:]");

            graphViz("list-grammar.dot");
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(ListParser);
    };
}

Y_UTEST(list)
{
    Syntax::Axiom::Verbose = true;
    ListParser parser;

    if(argc>1)
    {
        XNode::Pointer tree( parser.parseFile(argv[1]) );
        tree->graphViz("list.dot");
    }
}
Y_UTEST_DONE()

