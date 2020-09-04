
#include "y/utest/run.hpp"
#include "y/jive/lexer.hpp"
#include "y/jive/common-regexp.hpp"
#include "y/jive/lexical/plugin/single-line-comments.hpp"
#include "y/jive/lexical/plugin/multi-lines-comments.hpp"

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
            endl("endl",  "[:endl:]");
            jump("com1", "%", this, & Scanner::nothing );
            call("com2", "--", this, & Scanner::nothing);

            Scanner &com1 = decl( "com1" );
            com1.drop("chars",".");
            com1.jump( "Lexer", "[:endl:]", this, & Scanner::newLine );


            Scanner &com2 = decl( "com2" );
            com2.drop("chars",".");
            com2.back("[:endl:]", this, & Scanner::newLine );

            plug<ShellComments>("shellComments");
            plug<Lexical::SingleLineComments>("texComments",'%');

            plug<Lexical::MultiLinesComments>("xmlComments","<--!","-->");

        }


        virtual ~MyLexer() throw()
        {
        }

        void EnterCom1( const Token &token )
        {
            std::cerr << "<COM1 @" << token << ">";
        }

        void LeaveCom1(const Token &)
        {
            assert(origin);
            std::cerr << "<COM1/>";
            origin->newLine();
         }

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

    

}
Y_UTEST_DONE()
