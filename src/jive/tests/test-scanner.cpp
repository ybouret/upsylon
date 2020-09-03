#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/jive/lexical/scanner.hpp"
#include "y/jive/lexemes.hpp"
#include "y/jive/common-regexp.hpp"

using namespace upsylon;
using namespace Jive;

namespace {

    class MyScanner : public Lexical::Scanner
    {
    public:
        explicit MyScanner() : Lexical::Scanner("MyScanner", AcceptEOS)
        {
            emit("UINT",   RegExpFor::Unsigned);
            emit("INT",    RegExpFor::Integer);
            emit("ID",     RegExpFor::Identifier);
            emit("FLT",    RegExpFor::Float);
            emit("DBL",    RegExpFor::Double);

            drop("blanks","[:blank:]+");
            endl("endl",  "[:endl:]");

            jump("comment", "#", this, & Scanner::nothing );

        }

        virtual ~MyScanner() throw()
        {
        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(MyScanner);
    };
}

Y_UTEST(scanner)
{
    Y_UTEST_SIZEOF(Lexical::Rule);
    Y_UTEST_SIZEOF(Lexical::Scanner);

    Lexical::Scanner::Verbose = true;

    Lexemes   lexemes;

    {
        MyScanner scan;

        if(argc>1)
        {
        Source             source( Module::OpenFile(argv[1]) );
            Lexical::Directive directive = 0;
            {
                Lexeme *lx = 0;
                while( (lx=scan.probe(source,directive)) )
                {
                    lexemes.push_back(lx);
                }
            }
        }
    }

    Tags::Display();

    std::cerr << "<Lexemes>" << std::endl;
    for(const Lexeme *lx=lexemes.head;lx;lx=lx->next)
    {
        std::cerr << '<' << lx->label << '>' << " = " << "'" << *lx << "'" << "@" << lx->tag << ":" << lx->line << ":" << lx->column << std::endl;
    }
    std::cerr << "<Lexemes/>" << std::endl;


}
Y_UTEST_DONE()
