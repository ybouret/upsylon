
#include "y/jive/language/parser.hpp"
#include "y/jive/lexical/plugin/strings.hpp"
#include "y/utest/run.hpp"
#include "y/jive/common-regexp.hpp"
#include "y/jive/language/analyzer.hpp"

using namespace upsylon;
using namespace Jive;

namespace {

    class ENDL_Parser : public Language::Parser
    {
    public:
        virtual ~ENDL_Parser() throw() {}

        explicit ENDL_Parser() : Parser("ENDL")
        {
            Aggregate &self   = agg(label);
            Alternate &choice = alt("choice");

            choice << endline("CRLF","\\r\\n");
            choice << newline("CR", "\\r");
            choice << newline("LF", "\\n");
            choice << terminal("ID","[:alpha:]+");

            self << zeroOrMore(choice);
            drop("blank", "[:blank:]");

            graphViz("endl-grammar.dot");

            std::cerr << "CR:   " << getPattern("CR").toRegExp()   << std::endl;
            std::cerr << "LF:   " << getPattern("LF").toRegExp()   << std::endl;
            std::cerr << "CRLF: " << getPattern("CRLF").toRegExp() << std::endl;


            validate();
        }

    };

}

Y_UTEST(endl)
{
    Language::Axiom::Verbose = true;
    ENDL_Parser p;

    if(argc>1)
    {
        Source         source( Module::OpenFile(argv[1]));
        XNode::Pointer xnode( p.parse(source) );
        xnode->graphViz("endl.dot");
    }
}
Y_UTEST_DONE()

