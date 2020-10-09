
#include "y/jive/parser.hpp"
#include "y/jive/lexical/plugin/strings.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

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
            }
            
            drop("blank","[:blank:]");
            endl("endl", "[:endl:]");
            
            graphViz("json.dot");
            setRoot(ARRAY);
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
        XNode::Pointer xnode( json.parse(source) );
        
    }
}
Y_UTEST_DONE()
