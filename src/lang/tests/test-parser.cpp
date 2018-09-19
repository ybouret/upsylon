#include "y/lang/syntax/parser.hpp"
#include "y/lang/syntax/analyzer.hpp"
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
            AGG &json = agg("json");

            hook<Lexical::CXX_Comment>("C++ Comment");
            hook<Lexical::jString>("string");

            ALT &value    = alt("value");
            value << term("string") << sole("null") << sole("true") << sole("false") << term("number","-?[:digit:]+(\\.[:digit:]*)?([eE]-?[:digit:]+)?");

            RULE &COMA   = mark(',');
            
            ALT &jArray = alt("array");
            {
                RULE &LBRACK = mark("[","\\[");
                RULE &RBRACK = mark("]","\\]");
                {
                    AGG &EmptyArray = agg("empty_array");
                    EmptyArray << LBRACK << RBRACK;
                    jArray << EmptyArray;
                }
                {
                    AGG &HeavyArray = agg("heavy_array");
                    HeavyArray << LBRACK << value;
                    HeavyArray << zeroOrMore( agg("extra_value",true) << COMA << value );
                    HeavyArray << RBRACK;
                    jArray << HeavyArray;
                }
            }
            value << jArray;

            ALT &jObject = alt("object");
            {
                const Rule &LBRACE = mark("{","\\{");
                const Rule &RBRACE = mark("}","\\}");
                const Rule &COLUMN = mark(":",":");
                {
                    AGG &EmptyObject = agg("empty_object");
                    EmptyObject << LBRACE << RBRACE;
                    jObject << EmptyObject;
                }
                {
                    AGG &HeavyObject = agg("heavy_object");
                    HeavyObject << LBRACE;
                    {
                        AGG &jPair = agg("pair");
                        jPair << term("string") << COLUMN << value;
                        HeavyObject << jPair;
                        HeavyObject << zeroOrMore( agg("extra_pair",true) << COMA << jPair );
                    }
                    HeavyObject << RBRACE;
                    jObject << HeavyObject;
                }
            }
            value << jObject;
            
            json << choice(jArray,jObject);

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
        {
            ios::ocstream fp("tree.bin");
            ast->save(fp);
        }

        if(true)
        {
            Source reload( Module::OpenFile("tree.bin") );
            auto_ptr<Syntax::Node> node = Syntax::Node::Load(reload,json);
            node->GraphViz("tree2.dot");
        }

        Syntax::Analyzer A;
        A.walk(*ast);

    }
    
}
Y_UTEST_DONE()


