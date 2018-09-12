
#include "y/lang/lexical/translator.hpp"
#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace Lang;

namespace
{

}

Y_UTEST(lex)
{
    if(argc<=1 || 0!=strcmp(argv[1],"NULL") )
    {
        Source   source( Module::OpenSTDIN() );
        Lexeme::List           lexemes;
        Lexeme                 *lx  = 0;
        
        std::cerr << std::endl << "...done" << std::endl;

        for(lx=lexemes.head;lx;lx=lx->next)
        {
            std::cerr << lx->label << " : " << *lx << std::endl;
        }

    }
}
Y_UTEST_DONE()
