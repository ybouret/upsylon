

#include "y/jive/language/parser.hpp"
#include "y/jive/lexical/plugin/jstring.hpp"

#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Jive;

namespace
{

    class MyParser : public Language::Parser
    {
    public:

        MyParser() : Language::Parser("Sample")
        {
            const Terminal &ID  = terminal("ID", "[:alpha:]+");
            const Terminal &STR = plugin<Lexical::jString>("STR");
            setRoot( zeroOrMore(ID) );

            drop("[:blank:]", "[:blank:]" );
            drop_endl("[:endl:]", "[:endl:]");

            validate();
        }

        virtual ~MyParser() throw()
        {
        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(MyParser);
    };


}
#include "y/fs/local/fs.hpp"

Y_UTEST(lang_parser)
{
    Language::Axiom::Verbose = true;
    vfs &fs = local_fs::instance();
    fs.try_remove_file("tree.dot");
    fs.try_remove_file("tree.png");

    MyParser parser;

    if(argc>1)
    {
        Source source( Module::OpenFile(argv[1]));
        xTree  tree( parser.parse(source) );
        if( tree.is_valid() )
        {
            tree->graphViz("tree.dot");
        }
    }

}
Y_UTEST_DONE()

