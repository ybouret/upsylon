
#include "y/jargon/pattern/basic/all.hpp"
#include "y/jargon/pattern/dictionary.hpp"

#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"
#include "y/ios/icstream.hpp"

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
    Tester      test( (argc>1) ? argv[1] : NULL );
    Dictionary  dict;
    Y_CHECK( dict.insert("Any1",   Any1::Create()         ) ); Y_CHECK( dict.search("Any1")   );
    Y_CHECK( dict.insert("Single", Single::Create('a')    ) ); Y_CHECK( dict.search("Single") );
    Y_CHECK( dict.insert("Range",  Range::Create('a', 'z')) ); Y_CHECK( dict.search("Range")  );
    Y_CHECK( dict.insert("Excluded", Excluded::Create('a') ) ); Y_CHECK( dict.search("Excluded") );

    auto_ptr<Pattern> p = NULL;
    
    const bool success = dict.for_each( test ) ;
    Y_CHECK(success);
    
    
}
Y_UTEST_DONE()

