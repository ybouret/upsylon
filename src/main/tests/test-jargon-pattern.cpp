
#include "y/jargon/pattern/basic/all.hpp"
#include "y/jargon/pattern/logic/all.hpp"
#include "y/jargon/pattern/joker/all.hpp"
#include "y/jargon/pattern/dictionary.hpp"
#include "y/jargon/pattern/operands.hpp"

#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

#include "y/ios/icstream.hpp"
#include "y/ptr/auto.hpp"

#include "y/fs/local/file.hpp"

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
                const string binName = name + ".bin";
                const size_t written = p.save_to(binName);
                Y_CHECK( ios::local_file::length_of(binName) == written );
                {
                    ios::icstream fp(binName);
                    auto_ptr<Pattern> q = Pattern::Load(fp);
                    std::cerr << "save " << fourcc_(p.uuid) << std::endl;
                    std::cerr << "load " << fourcc_(q->uuid) << std::endl;
                    Y_CHECK( q->alike(&p) );
                }
                
            }
            
            {
                const string dotFile = name + ".dot";
                p.graphViz(dotFile);
            }
            
            if(p.strong())
            {
                p.test(source, content); std::cerr << std::endl;
            }
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
        Y_CHECK( dict.insert("And",p.yield())   ); Y_CHECK( dict.search("And") );
    }
    
    {
        auto_ptr<Logical> p = OR::Create();
        p->push_back( Single::Create('a') );
        p->push_back( Single::Create('b') );
        p->push_back( Single::Create('a') );
        Y_CHECK( dict.insert("Or",p.yield())   ); Y_CHECK( dict.search("Or") );
    }
    
    {
        auto_ptr<Logical> p = NONE::Create();
        p->push_back( Single::Create('a') );
        p->push_back( Single::Create('b') );
        p->push_back( Single::Create('a') );
        Y_CHECK( dict.insert("None",p.yield())   ); Y_CHECK( dict.search("None") );
    }
    
    {
        auto_ptr<Pattern> p = Optional::Create( Single::Create('a') );
        Y_CHECK( dict.insert("Opt",p.yield())   ); Y_CHECK( dict.search("Opt") );
    }
    
    {
        auto_ptr<Logical> ab = AND::Create();
        ab->push_back( Single::Create( 'a' ) );
        ab->push_back( Optional::Create( Single::Create('b') ) );
        Y_CHECK( dict.insert("ab",ab.yield()) );Y_CHECK( dict.search("ab") );
    }
    
    {
        auto_ptr<Pattern> p = Repeating::ZeroOrMore( Single::Create('a') );
        Y_CHECK( dict.insert("ZOM",p.yield())   ); Y_CHECK( dict.search("ZOM") );
    }
    
    {
        auto_ptr<Pattern> p = Repeating::OneOrMore( Single::Create('a') );
        Y_CHECK( dict.insert("OOM",p.yield())   ); Y_CHECK( dict.search("OOM") );
    }
    
    {
        auto_ptr<Pattern> p = Repeating::Create( Single::Create('a'), 2 );
        Y_CHECK( dict.insert("Rep",p.yield())   ); Y_CHECK( dict.search("Rep") );
    }
    
    {
        auto_ptr<Pattern> p = Counting::Create( Single::Create('a'), 0,0 );
        Y_CHECK( dict.insert("Cnt00",p.yield())   ); Y_CHECK( dict.search("Cnt00") );
    }
    
    {
        auto_ptr<Pattern> p = Counting::Create( Single::Create('a'), 0,1 );
        Y_CHECK( dict.insert("Cnt01",p.yield())   ); Y_CHECK( dict.search("Cnt01") );
    }
    
    {
        auto_ptr<Pattern> p = Counting::Create( Single::Create('a'), 0,2 );
        Y_CHECK( dict.insert("Cnt02",p.yield())   ); Y_CHECK( dict.search("Cnt02") );
    }
    
    {
        auto_ptr<Pattern> p = Counting::Create( Single::Create('a'), 1,5 );
        Y_CHECK( dict.insert("Cnt15",p.yield())   ); Y_CHECK( dict.search("Cnt15") );
    }
    
    
    const bool jargon_pattern_success = dict.for_each( test ) ;
    Y_CHECK(jargon_pattern_success);
    
    
}
Y_UTEST_DONE()

