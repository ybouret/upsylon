
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

            Aggregate & value = agg("value");
            value << terminal("null");
            value << terminal("true");
            value << terminal("false");

            drop("blank","[:blanks:]");
            endl("endl", "[:endl:]");
        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(JSON_Parser);
    };

}


Y_UTEST(parser)
{

}
Y_UTEST_DONE()
