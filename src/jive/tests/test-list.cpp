#include "y/jive/language/parser.hpp"
#include "y/utest/run.hpp"
#include "y/jive/lexical/plugin/single-line-comments.hpp"
#include "y/jive/language/analyzer.hpp"

using namespace upsylon;
using namespace Jive;

namespace
{
    class ListParser : public Language::Parser
    {
    public:
        inline virtual ~ListParser() throw()
        {


        }

        inline explicit ListParser() : Language::Parser("Lists")
        {
            Aggregate   &LIST     = agg("list");
            const Axiom &NAME     = terminal("NAME","[:word:]+");
            {
                Aggregate &ELEMENTS = grp("elements");
                LIST << division('[') << ELEMENTS << division(']');
                Compound &ELEMENT  = alt("element") << NAME << LIST;
                ELEMENTS << ELEMENT << zeroOrMore( cat( Axioms(division(','), ELEMENT) ) );
            }

            // lexical only
            plug<Lexical::SingleLineComments>("COMMENT","//");
            drop("blank","[:blank:]");
            drop_endl("endl","[:endl:]");

            graphViz("list-grammar.dot");
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(ListParser);
    };
}

Y_UTEST(list)
{
    Language::Axiom::Verbose = true;
    ListParser parser;

    if(argc>1)
    {
        XNode::Pointer tree( parser.parseFile(argv[1]) );
        tree->graphViz("list.dot");
        Language::Analyzer analyzer( parser.name );
        
        analyzer.walk( tree.content() );

    }
}
Y_UTEST_DONE()

