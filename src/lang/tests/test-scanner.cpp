
#include "y/lang/lexical/scanner.hpp"
#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace Lang;

namespace
{
    class localScanner : public Lexical::Scanner
    {
    public:
        explicit localScanner() : Lexical::Scanner("localScanner")
        {
            emit("id","[:alpha:]+");
            emit("int","[:digit:]+");
            emit("dbl","[:digit:]+\\.[:digit:]*");
            drop("blanks","[:blank:]+");
            endl("endl","[:endl:]");
        }

        virtual ~localScanner() throw()
        {
        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(localScanner);
    };
}

Y_UTEST(scanner)
{
#if 0
    localScanner scanner;
    if(argc<=1 || 0!=strcmp(argv[1],"NULL") )
    {
        std::cerr << "Scanning..." << std::endl;
        Source   source( Module::OpenSTDIN() );
        Lexeme::List  lexemes;
        Lexeme       *lx = 0;
        Lexical::Result result = Lexical::Forward;
        while(NULL!=(lx=scanner.probe(source,result)))
        {
            lexemes.push_back(lx);
        }
        std::cerr << std::endl << "done" << std::endl;

        for(lx=lexemes.head;lx;lx=lx->next)
        {
            std::cerr << *lx << std::endl;
        }
        
    }
#endif
}
Y_UTEST_DONE()

