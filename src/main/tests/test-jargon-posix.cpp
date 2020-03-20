#include "y/jargon/pattern/posix.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"

using namespace upsylon;
using namespace Jargon;

namespace {
    
    class Tester
    {
    public:
        Char::Cache cache;
        Source      source;
        Token       content;
        
        inline Tester(const char *filename) :
        cache( new Char::Pool() ),
        source( cache, (NULL!=filename) ? Module::OpenFile(cache,filename) : Module::OpenData(cache,"NULL", 0,0) ),
        content(cache)
        {
        }
        
        inline ~Tester() throw()
        {
        }
      
        inline void run(const Pattern &p)
        {
            p.test(source,content);
            source.unget(content);
        }
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(Tester);
    };
    
}


#define Y_JPOSIX(EXPR)                     \
do {                                       \
std::cerr << "\t\t<posix::" #EXPR ">" << std::endl; \
auto_ptr<Pattern> p = posix::EXPR();       \
Y_CHECK( p->strong()  );                   \
Y_CHECK( p->checkIO() );                   \
p->graphViz( #EXPR ".dot" );              \
test.run(*p);                            \
std::cerr << std::endl;                 \
std::cerr << std::endl;                \
} while(false)

Y_UTEST(jargon_posix)
{
    Tester test( (argc>1) ? argv[1] : NULL );
    
    Y_JPOSIX(lower);
    Y_JPOSIX(upper);
    Y_JPOSIX(alpha);
    Y_JPOSIX(digit);
    Y_JPOSIX(xdigit);
    Y_JPOSIX(alnum);
    Y_JPOSIX(blank);
    Y_JPOSIX(space);
    Y_JPOSIX(punct);
    
    Y_JPOSIX(word);
    Y_JPOSIX(endl);
    Y_JPOSIX(dot);
    Y_JPOSIX(core);
    
}
Y_UTEST_DONE()

