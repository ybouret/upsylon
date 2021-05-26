
#include "y/jive/parser.hpp"
#include "y/jive/lexical/plugin/strings.hpp"
#include "y/utest/run.hpp"
#include "y/jive/common-regexp.hpp"
#include "y/jive/syntax/analyzer.hpp"

using namespace upsylon;
using namespace Jive;

namespace {

    class ENDL_Parser : public Parser
    {
    public:
        virtual ~ENDL_Parser() throw() {}

        explicit ENDL_Parser() : Parser("ENDL")
        {
            Aggregate &self   = agg(label);
            Alternate &choice = alt("choice");

            choice << new_line("CRLF","\\r\\n");
            choice << new_line("CR", "\\r");
            choice << new_line("LF", "\\n");
            choice << terminal("ID","[:alpha:]+");

            self << zeroOrMore(choice);
            drop("blank", "[:blank:]");

            graphViz("endl-grammar.dot");

            std::cerr << "CR:   " << pattern_for("CR").to_printable() << std::endl;
            std::cerr << "LF:   " << pattern_for("LF").to_printable() << std::endl;
            std::cerr << "CRLF: " << pattern_for("CRLF").to_printable() << std::endl;


            validate();
        }

    };

}

Y_UTEST(endl)
{
    Syntax::Axiom::Verbose = true;
    ENDL_Parser p;

    if(argc>1)
    {
        Source         source( Module::OpenFile(argv[1]));
        XNode::Pointer xnode( p.parse(source) );
        xnode->graphViz("endl.dot");
    }
}
Y_UTEST_DONE()

