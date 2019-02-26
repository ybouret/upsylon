
#include "y/lang/lexical/translator.hpp"
#include "y/lang/lexical/plugin/comment.hpp"
#include "y/lang/lexical/plugin/strings.hpp"

#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace Lang;

namespace
{
    class myLex : public Lexical::Translator
    {
    public:
        explicit myLex() : Lexical::Translator("myLex")
        {

            Lexical::Scanner &root = **this;
            root.verbose = true;
            root.emit("id","[:alpha:]+");
            root.emit("int","[:digit:]+");
            root.drop("blanks","[:blank:]+");

            root.jump("COM1", "#",    this, & myLex::COM1_Enter);
            root.call("COM2", "<!--", this, & myLex::COM2_Enter);
            hook<Lexical::CXX_Comment>(root,"CXX");
            hook<Lexical::EndOfLineComment>(root,"LatexComment","%");
            hook<Lexical::C_Comment>(root,"C_Comment");
            hook<Lexical::jString>(root,"jstring");
            hook<Lexical::rString>(root,"rstring");
            root.endl("endl", "[:endl:]");

            Lexical::Scanner &COM1 = decl("COM1");
            COM1.drop("any",".");
            COM1.jump("myLex", "[:endl:]",this, & myLex::COM1_Leave);

            Lexical::Scanner &COM2 = decl("COM2");
            COM2.back("-->", this, &myLex::COM2_Leave);
            COM2.drop("any",".");
            COM2.endl("endl","[:endl:]");

            hook<Lexical::MultiLinesComment>(root,"XMLCom", "<--", "-->");

        }

        virtual ~myLex() throw()
        {
        }

        void COM1_Enter(const Token &) throw()
        {
            std::cerr << "<COM1>" << std::endl;
        }

        void COM1_Leave(const Token &_) throw()
        {
            std::cerr << "<COM1/>" << std::endl;
            (**this).newline(_);
        }

        void COM2_Enter(const Token &) throw()
        {
            std::cerr << "<COM2>" << std::endl;
        }

        void COM2_Leave(const Token &) throw()
        {
            std::cerr << "<COM2/>" << std::endl;
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(myLex);
    };
}

Y_UTEST(lex)
{
    myLex lex;
    if(argc<=1 || 0!=strcmp(argv[1],"NULL") )
    {
        std::cerr << "Start Translator [" << lex.label << "]" << std::endl;
        Source   source( Module::OpenSTDIN() );
        Lexeme::List           lexemes;
        Lexeme                 *lx  = 0;
        while( NULL != (lx=lex.get(source) ) )
        {
            lexemes.push_back(lx);
        }
        std::cerr << std::endl << "...done" << std::endl;

        std::cerr << "#lexemes=" << lexemes.size << std::endl;
        for(lx=lexemes.head;lx;lx=lx->next)
        {
            std::cerr << lx->label << " : " << lx->to_print() << "@" << lx->line() << ":" << lx->column() << std::endl;
        }

    }
}
Y_UTEST_DONE()
