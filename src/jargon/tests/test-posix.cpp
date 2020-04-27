#include "y/jargon/pattern/posix.hpp"
#include "y/jargon/pattern/regexp.hpp"
#include "y/associative/map.hpp"

#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"
#include "y/jargon/first-chars.hpp"

using namespace upsylon;
using namespace Jargon;

namespace {
    
    class Tester
    {
    public:
        Source      source;
        Token       content;
        
        inline Tester(const char *filename) :
        source( (NULL!=filename) ? Module::OpenFile(filename) : Module::OpenData("NULL", 0,0) ),
        content()
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


#define Y_JPOSIX(EXPR)                            \
do {                                              \
const string id = #EXPR; fc.free();               \
std::cerr << "\t\tposix::" << id  << std::endl;   \
auto_ptr<Pattern> p = posix::EXPR();              \
Y_CHECK( p->strong()  );                         \
Y_CHECK( p->checkIO() );                        \
auto_ptr<Pattern> q = posix::get(#EXPR);       \
Y_CHECK( p->alike( & *q) );                   \
p->graphViz( #EXPR ".dot" );                 \
test.run(*p);                               \
all.push_back( p.yield() );                \
const Motif m = all.tail->clone();        \
Y_ASSERT(db.insert(id,m));               \
m->adjoin(fc);                          \
std::cerr << "fc=" << fc << std::endl; \
const string rx = m->toRegExp();       \
std::cerr << "rx=" << rx << std::endl; \
q = RegularExpression::Compile(rx);    \
Y_ASSERT(q->toRegExp()==rx);           \
std::cerr << std::endl;                \
std::cerr << std::endl;                \
} while(false)

Y_UTEST(posix)
{
    Tester    test( (argc>1) ? argv[1] : NULL );
    typedef map<string,Motif> db_type;
    db_type           db;
    Pattern::List     all;
    FirstChars        fc(256);
    
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
    
    std::cerr << "#patterns: " << all.size << std::endl;
    Pattern::SortByEntropy(all);
    for(const Pattern *p=all.head;p;p=p->next)
    {
        std::cerr << "\t$" << p->entropy << std::endl;
    }
    std::cerr << "#db: " << db.size() << std::endl;
    for(db_type::iterator it=db.begin();it!=db.end();++it)
    {
        const Pattern &p = **it;
        p.updateEntropy();
        std::cerr << "\t" << it.key() << " $" << (*it)->entropy << std::endl;
    }
}
Y_UTEST_DONE()

