
#include "y/lang/syntax/parser.hpp"
#include "y/utest/run.hpp"
#include "y/lang/lexical/plugin/strings.hpp"
#include "y/lang/syntax/analyzer.hpp"

using namespace upsylon;
using namespace Lang;

namespace {

    class jParser : public Syntax::Parser
    {
    public:
        inline jParser() : Syntax::Parser("JSON")
        {
            setVerbose(true);


            // lexical rules
        }

        inline virtual ~jParser() throw()
        {

        }

    };

}

Y_UTEST(json)
{
    Syntax::Parser::Pointer P = new jParser();
    std::cerr << "Building [" << P->key() << "]" << std::endl;
    if( argc>1 && 0 == strcmp("run",argv[1]) )
    {
        Source source( Module::OpenSTDIN() );
        auto_ptr<Syntax::Node> ast = P->run( source );
        ast->graphViz( *(P->name) + "_tree.dot" );

        const string binfile = *(P->name) + "_tree.bin";
        ast->save(binfile);

        auto_ptr<Syntax::Node> reloaded = Syntax::Node::Load( Module::OpenFile(binfile),*P);
        const string ast64 = ast->toBase64();
        const string bin64 = reloaded->toBase64();
        std::cerr << "ast: " << ast64 << std::endl;
        std::cerr << "bin: " << bin64 << std::endl;
        Y_CHECK(ast64==bin64);

        Syntax::Analyzer a;
        a.walk(*ast);

    }
}
Y_UTEST_DONE()
