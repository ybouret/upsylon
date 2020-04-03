
#include "y/jargon/pattern/regexp.hpp"
#include "y/jargon/pattern/dictionary.hpp"

#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"
#include "y/fs/local/file.hpp"

using namespace upsylon;
using namespace Jargon;

namespace {
    
}

Y_UTEST(jargon_regexp)
{
    Dictionary dict;
    //RegularExpression::Verbose = true;


    Y_CHECK(dict.insert("INT","[:digit:]+"));
    if(argc>1)
    {
        const string      rx = argv[1];
        auto_ptr<Pattern> p  = RegularExpression::Compile(rx,&dict); Y_CHECK(p.is_valid());
        const size_t written = p->save_to("regexp.bin");
        Y_CHECK( ios::local_file::length_of("regexp.bin") == written );
       
        p->graphViz("regexp.dot");
        p->updateEntropy();
        std::cerr << "strong   : " << p->strong()   << std::endl;
        std::cerr << "univocal : " << p->univocal() << std::endl;
        std::cerr << "entropy  : " << p->entropy    << std::endl;
        
        if(argc>2)
        {
            Cache  cache;
            Source source(Module::OpenFile(cache, argv[2]));
            Token  content(cache);
            p->test(source,content);
        }
        
        
    }
    
}
Y_UTEST_DONE()

Y_UTEST(jargon_regcom)
{
#define Y_JRX(EXPR) do { const Motif p = RegularExpression::Compile( RegularExpression::EXPR );\
p->graphViz( #EXPR ".dot" ); p->save_to( #EXPR ".bin" ); com.push_back(p); rid.push_back(#EXPR); } while(false)
    
    vector<Motif>  com;
    vector<string> rid;
    
    Y_JRX(Integer);
    Y_JRX(Real);
    Y_JRX(Identifier);

    if(argc>1)
    {
        Cache  cache;
        Source source(Module::OpenFile(cache, argv[1]));
        Token  content(cache);
        
        for(size_t i=1;i<=com.size();++i)
        {
            std::cerr << "Testing <" << rid[i] << ">" << std::endl;
            com[i]->test(source,content);
            source.unget(content);
        }
    }
    

}
Y_UTEST_DONE()


