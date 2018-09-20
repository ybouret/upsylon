
#include "y/lang/compiler.hpp"
#include "y/lang/lexical/plugin/strings.hpp"

#include "y/utest/run.hpp"
#include "y/fs/local.hpp"

using namespace upsylon;
using namespace Lang;

namespace
{
    class JSON_Parser : public Syntax::Parser
    {
    public:
        explicit JSON_Parser() : Syntax::Parser("JSON")
        {
            AGG &json = agg("json");

            ALT &value = alt("value");
            hook<Lexical::jString>("string");
            value << sole("null") << sole("true") << sole("false") << term("number","-?[:digit:]+(\\.[:digit:]*)?([eE]-?[:digit:]+)?") << term("string");

            ALT &jArray = alt("array");
            {
                jArray << (agg("empty_array") << mark('[') << mark(']'));
                jArray << (agg("heavy_array")
                           << mark('[')
                           << value
                           << zeroOrMore( acting("extra_value") << mark(',') << value  )
                           << mark(']'));
            }

            ALT &jObject = alt("object");
            {
                jObject << (agg("empty_object") << mark('{') << mark('}') );
                {
                    COMPOUND &jPair = (agg("pair") << term("string") << mark(':') << value);
                    jObject << ( agg("heavy_object")
                                << mark('{') << jPair
                                << zeroOrMore( acting("extra_pair") << mark(',') << jPair)
                                << mark('}')
                                );
                }
            }

            value << jArray << jObject;

            json << choice(jArray,jObject);

            root.endl("endl", "[:endl:]");
            root.drop("ws","[:blank:]");

            checkValidity();
            GraphViz("json.dot");

        }

        virtual ~JSON_Parser() throw()
        {
        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(JSON_Parser);
    };

    class JSON_Compiler : public Compiler
    {
    public:
        explicit JSON_Compiler() : Compiler( new JSON_Parser() )
        {

        }

        virtual ~JSON_Compiler() throw()
        {
        }
        

    private:
        Y_DISABLE_COPY_AND_ASSIGN(JSON_Compiler);
    };
}

Y_UTEST(json)
{
    JSON_Compiler json;
    Module *module = 0;
    if(argc>1)
    {
        const string filename = argv[1];
        if("NULL"==filename) return 0;
        module = Module::OpenFile(filename);
    }
    else
    {
        module = Module::OpenSTDIN();
    }

    json.compile(module,0);
    assert(json.cst.is_valid());
    json.cst->GraphViz("json-cst.dot");
}
Y_UTEST_DONE()

