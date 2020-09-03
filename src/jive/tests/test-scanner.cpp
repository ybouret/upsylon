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
            emit("INT",    RegExpFor::Integer);
            emit("UINT",   RegExpFor::Unsigned);

            drop("blanks","[:blank:]+");
            endl("endl",  "[:endl:]");
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

    MyScanner scan;

    if(argc>1)
    {
        Source             source( Module::OpenFile(argv[1]) );
        Lexical::Directive directive = 0;
        scan.probe(source,directive);
        
    }



}
Y_UTEST_DONE()
