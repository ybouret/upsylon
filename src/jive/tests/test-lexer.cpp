
#include "y/utest/run.hpp"
#include "y/jive/lexer.hpp"
#include "y/jive/common-regexp.hpp"
#include "y/jive/lexical/plugin/comments.hpp"
#include "y/jive/lexical/plugin/strings.hpp"
#include "y/jive/lexical/plugin/block-reader.hpp"

using namespace upsylon;
using namespace Jive;

namespace {

    Y_JIVE_LEXICAL_SL_COMMENTS(LuaComments,"--");

    class MyLexer : public Lexer
    {
    public:
        explicit MyLexer() : Lexer( "Lexer" )
        {
            emit("ID",     RegExpFor::Identifier);
            drop("blanks","[:blank:]+");
            silent_endl("endl",  "[:endl:]");
            jump("com1", "%",  this, & MyLexer::EnterCom1);
            call("com2", "--", this, & Scanner::nothing);

            {
                Scanner &com1 = decl( "com1" );
                com1.drop("chars",".");
                com1.jump( "Lexer", "[:endl:]", &com1, & MyLexer::newLine );
            }


            Scanner &com2 = decl( "com2" );
            com2.drop("chars",".");
            com2.back("[:endl:]", &com2, & Scanner::newLine );

            call( plug<ShellComments>("shellComments") );
            call( plug<CppComments>("cppComments") );

            //call( plug<Lexical::SingleLineComments>("texComments",'%') );
            //call( plug<LuaComments>("luaComments") );

            call( plug<Lexical::MultiLinesComments>("xmlComments","<--!","-->"));
            call( plug<C_Comments>("C_Comments") );

            call( plug<Lexical::jString>("jstring")   );
            call( plug<Lexical::rString>("rstring") );
            call( plug<Lexical::bString>("bstring") );


            {
                Y_CHECK( hasPlugin("jstring") );
                const string rstring = "rstring";
                Y_CHECK( hasPlugin(rstring) );
                Plugin &xmlComments = getPlugin("xmlComments");
                Y_CHECK(*xmlComments.label=="xmlComments");
            }

            call( plug<Lexical::BlockReader>("block","\\{", "\\}"));

        }


        virtual ~MyLexer() throw()
        {
        }

        void EnterCom1( const Token &token )
        {
            std::cerr << "<COM1 @" << token << ">";
        }

#if 0
        void LeaveCom1(const Token &)
        {
            assert(origin);
            std::cerr << "<COM1/>";
            origin->newLine();
         }
#endif
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(MyLexer);
    };
}

Y_UTEST(lexer)
{
    Lexical::Scanner::Verbose = true;
    Lexemes lexemes;
    MyLexer lexer;
    if(argc>1)
    {
        Source source( Module::OpenFile(argv[1]));
        lexer.ready(source,8);
        Lexeme *lx = 0;
        while( (lx=lexer.get(source)) )
        {
            lexemes.push_back(lx);
        }
    }

    std::cerr << "<Lexemes>" << std::endl;
    for(const Lexeme *lx=lexemes.head;lx;lx=lx->next)
    {
        std::cerr << "\t<" << lx->label << "> '" << *lx << "' @" << lx->tag << ":" << lx->line << ":" << lx->column << std::endl;
    }
    std::cerr << "<Lexemes/>" << std::endl;

    

}
Y_UTEST_DONE()
