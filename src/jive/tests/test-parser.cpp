
#include "y/jive/parser.hpp"
#include "y/jive/lexical/plugin/strings.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/jive/common-regexp.hpp"

using namespace upsylon;
using namespace Jive;

namespace {

    class JSON_Parser : public Parser
    {
    public:
        virtual ~JSON_Parser() throw() {}

        explicit JSON_Parser() : Parser("JSON")
        {

            const Terminal &STRING = plugin<Lexical::jString>("string");
            Alternate      &VALUE  = alt("value");
            VALUE << terminal("null");
            VALUE << terminal("true");
            VALUE << terminal("false");
            VALUE << terminal("number",RegExpFor::Double);
            VALUE << STRING;
            
            const Terminal &COMA  = division(',');
            Alternate      &ARRAY = alt("array");
            {
                const Axiom &LBRACK = division('[');
                const Axiom &RBRACK = division(']');
                ARRAY << ( agg("empty_array") << LBRACK << RBRACK);
                ARRAY << (
                          agg("heavy_array") << LBRACK << VALUE
                          << repeat( cat(COMA,VALUE), 0)
                          << RBRACK
                          );
                VALUE << ARRAY;
            }

            Alternate  &OBJECT = alt("object");
            {
                const Axiom &LBRACE = division('{');
                const Axiom &RBRACE = division('}');
                OBJECT << ( agg("empty_object") << LBRACE << RBRACE);
                const Axiom &PAIR   = agg("pair") << STRING << division(':') << VALUE;
                OBJECT << (
                           agg("heavy_object") << LBRACE << PAIR << repeat( cat(COMA,PAIR), 0 ) << RBRACE
                           );
                VALUE << OBJECT;
            }

            drop("blank","[:blank:]");
            endl("endl", "[:endl:]");
            
            setRoot( alt("JSON") << ARRAY << OBJECT );
            graphViz("json.dot");
            validate();
        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(JSON_Parser);
    };

}


Y_UTEST(parser)
{
    JSON_Parser json;
    if(argc>1)
    {
        Source         source( Module::OpenFile(argv[1]));
        Syntax::Axiom::Verbose = true;
        XNode::Pointer xnode( json.parse(source) );
        if( xnode.is_empty() )
        {
            throw exception("empty node");
        }
        else
        {
            xnode->graphViz("jtree.dot");
        }
    }
}
Y_UTEST_DONE()
