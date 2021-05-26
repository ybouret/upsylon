
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

            Alternate &self = alt("top");

            self << end_line("CRLF","\\r\\n");
            self << end_line("CR", "\\r");
            self << end_line("LF", "\\n");
            self << terminal("ID","[:alpha:]+");
            drop("blank", "[:blank:]");

            graphViz("endl-grammar.dot");

            std::cerr << "CR:" << pattern_for("CR").to_printable() << std::endl;
            std::cerr << "LF:" << pattern_for("LF").to_printable() << std::endl;


            validate();
        }

    };

}

Y_UTEST(endl)
{
    ENDL_Parser p;
}
Y_UTEST_DONE()

