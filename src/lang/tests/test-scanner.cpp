
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
            //if(false)
            {
                emit("id","[:alpha:]+");
                emit("int","[:digit:]+");
                emit("dbl","[:digit:]+\\.[:digit:]*");
                drop("blanks","[:blank:]+");
                endl("endl","[:endl:]");
            }
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
    localScanner scanner;
    if(argc<=1 || 0!=strcmp(argv[1],"NULL") )
    {
        std::cerr << "Scanning..." << std::endl;
        Source   source( Module::OpenSTDIN() );
        Lexeme::List           lexemes;
        Lexeme                 *lx  = 0;
        Lexical::Message        msg = 0;
        while(NULL!=(lx=scanner.probe(source,msg)))
        {
            lexemes.push_back(lx);
        }
        std::cerr << std::endl << "...done" << std::endl;

        for(lx=lexemes.head;lx;lx=lx->next)
        {
            std::cerr << lx->label << " : " << *lx << std::endl;
        }
        
    }
}
Y_UTEST_DONE()

