#include "y/lang/syntax/parser.hpp"
#include "y/lang/lexical/plugin/comment.hpp"
#include "y/lang/lexical/plugin/strings.hpp"

#include "y/utest/run.hpp"
#include "y/fs/local.hpp"

using namespace upsylon;
using namespace Lang;

namespace
{
    class myParser : public Syntax::Parser
    {
    public:
        explicit myParser() : Syntax::Parser("JSON")
        {
            Agg &json = agg("json");

            hook<Lexical::CXX_Comment>("C++ Comment");
            hook<Lexical::jString>("string");

            Alt &value    = alt("value");
            value << term("string") << sole("null") << sole("true") << sole("false");


            Alt &Array = alt("array");
            {
                const Rule &LBRACK = mark("[","\\[");
                const Rule &RBRACK = mark("]","\\]");
                const Rule &COMA   = mark(",",",");
                {
                    Agg &EmptyArray = agg("empty_array");
                    EmptyArray << LBRACK << RBRACK;
                    Array << EmptyArray;
                }
                {
                    Agg &HeavyArray = agg("heavy_array");
                    HeavyArray << LBRACK << value;
                    HeavyArray << zeroOrMore( agg("extra_value") << COMA << value );
                    HeavyArray << RBRACK;
                    Array << HeavyArray;
                }
            }
            value << Array;
            json << Array;

            // extra lexical rules
            root.endl("endl", "[:endl:]");
            root.drop("ws","[:blank:]");

            checkValidity();
            GraphViz("json.dot");

        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(myParser);
    };
}

Y_UTEST(parser)
{
    vfs &fs = local_fs::instance();
    fs.try_remove_file("json.png");
    fs.try_remove_file("tree.png");

    myParser json;
    if(argc>1&&0==strcmp(argv[1],"run"))
    {
        Source source( Module::OpenSTDIN() );
        auto_ptr<Syntax::Node> ast = json.parse(source);
        ast->GraphViz("tree.dot");
    }
    
}
Y_UTEST_DONE()


