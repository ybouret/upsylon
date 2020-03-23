
#include "y/jargon/pattern/regexp.hpp"
#include "y/jargon/lexical/scanner.hpp"
#include "y/jargon/lexeme.hpp"

#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;
using namespace Jargon;

namespace {
    
    class myScanner : public Lexical::Scanner
    {
    public:
        myScanner() : Lexical::Scanner("myScan")
        {
            const string alpha  = "alpha";
            const string alphas = "[:alpha:]+";
            
            emit(alpha,alphas);
            emit("double","[:digit:]+");
            emit("float","[:digit:]+f");

            const string xrx = "0x[:xdigit:]+";
            emit("hexa",xrx);
            
            const string blanks = "blanks";
            drop(blanks,"[:blank:]");
            
            endl("endl","[:endl:]");
            
        }
        
        virtual ~myScanner() throw()
        {
        }
        
        
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(myScanner);
    };
    
}

Y_UTEST(jargon_scanner)
{
    Y_UTEST_SIZEOF(Lexeme);
    Y_UTEST_SIZEOF(Lexemes);
    Y_UTEST_SIZEOF(Lexical::Scanner);

    myScanner scan;
    
    {
        scan.finish();
        scan.resume();
        scan.finish();
    }
    
    if(argc>1)
    {
        Lexemes   lexemes;
        Cache     cache;
        Source    source(cache,Module::OpenFile(cache, argv[1]));
        Lexeme   *lx   = 0;
        Lexical::Directive ctrl = 0;
        while( NULL != (lx = scan.probe(source,ctrl)) )
        {
            lexemes.push_back(lx);
        }
        while( lexemes.size )
        {
            lx = lexemes.head;
            std::cerr << "[" << lx->label << "] '" << *lx << "' @" << lx->tag << ":" << lx->line << ":" << lx->column << std::endl;
            delete lexemes.pop_front();
        }
    }
    
}
Y_UTEST_DONE()

