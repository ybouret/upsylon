#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/jive/lexical/scanner.hpp"

using namespace upsylon;
using namespace Jive;

namespace {

    class MyScanner : public Lexical::Scanner
    {
    public:
        explicit MyScanner() : Lexical::Scanner("MyScanner")
        {

            emit("INT",   "[:digit:]+");
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

    MyScanner scan;


}
Y_UTEST_DONE()
