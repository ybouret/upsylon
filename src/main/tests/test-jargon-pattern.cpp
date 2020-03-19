
#include "y/jargon/pattern/basic/all.hpp"
#include "y/jargon/pattern/logic/all.hpp"
#include "y/jargon/pattern/dictionary.hpp"
#include "y/jargon/pattern/operands.hpp"

#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

#include "y/ios/icstream.hpp"
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
        
        Tester(const char *filename) :
        cache( new Char::Pool() ),
        source( cache, (NULL!=filename) ? Module::OpenFile(cache,filename) : Module::OpenData(cache,"NULL", 0,0) ),
        content(cache)
        {
        }
        
        ~Tester() throw()
        {
        }
        
        void run(const Pattern &p)
        {
            std::cerr << "testing <" << p.className() << ">" << std::endl;
            
        }
        
        bool operator()( const suffix_path &key, const Motif &m )
        {
            const Pattern &p    = *m;
            const string   name = core::suffix_tree::path2string(key);
            std::cerr << "Testing " << name << " as <" << p.className() << ">" << std::endl;
            std::cerr << "|_feeble:   " << p.feeble()   << std::endl;
            std::cerr << "|_strong:   " << p.strong()   << std::endl;
            std::cerr << "|_multiple: " << p.multiple() << std::endl;
            std::cerr << "|_univocal: " << p.univocal() << std::endl;
            {
                const string binName = string("p") + p.className() + ".bin";
                p.save_to(binName);
                {
                    ios::icstream fp(binName);
                    auto_ptr<Pattern> q = Pattern::Load(fp);
                    Y_ASSERT( q->alike(&p) );
                }
            }
            
            {
                const string dotFile = name + ".dot";
                p.graphViz(dotFile);
            }
            
            if(p.strong())
                p.test(source, content);
            
            source.unget(content);
            return true;
        }
        
        
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(Tester);
    };
    
    
}

Y_UTEST(jargon_pattern)
{
    Y_UTEST_SIZEOF( Pattern  );
    Y_UTEST_SIZEOF( Any1     );
    Y_UTEST_SIZEOF( Single   );
    Y_UTEST_SIZEOF( Range    );
    Y_UTEST_SIZEOF( Excluded );
    
    Tester      test( (argc>1) ? argv[1] : NULL );
    Dictionary  dict;
    Y_CHECK( dict.insert("Any1",     Any1::Create()          ) ); Y_CHECK( dict.search("Any1")     );
    Y_CHECK( dict.insert("Single",   Single::Create('a')     ) ); Y_CHECK( dict.search("Single")   );
    Y_CHECK( dict.insert("Range",    Range::Create('a', 'z') ) ); Y_CHECK( dict.search("Range")    );
    Y_CHECK( dict.insert("Excluded", Excluded::Create('a') )   ); Y_CHECK( dict.search("Excluded") );
    
    {
        auto_ptr<Logical> p = AND::Create();
        p->push_back( Single::Create('a') );
        p->push_back( Single::Create('b') );
        p->simplify();
        Y_CHECK( dict.insert("And",p.yield())   ); Y_CHECK( dict.search("And") );
    }
    
    {
        auto_ptr<Logical> p = OR::Create();
        p->push_back( Single::Create('a') );
        p->push_back( Single::Create('b') );
        p->push_back( Single::Create('a') );
        p->simplify();
        Y_CHECK( dict.insert("Or",p.yield())   ); Y_CHECK( dict.search("Or") );
    }
    
    
    const bool jargon_pattern_success = dict.for_each( test ) ;
    Y_CHECK(jargon_pattern_success);
    
    
}
Y_UTEST_DONE()

