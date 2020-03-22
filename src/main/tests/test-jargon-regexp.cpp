
#include "y/jargon/pattern/regexp.hpp"
#include "y/jargon/pattern/dictionary.hpp"

#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"
#include "y/fs/local/file.hpp"

using namespace upsylon;
using namespace Jargon;

namespace {
    
}

#include "y/information/entropy.hpp"

Y_UTEST(jargon_regexp)
{
    Dictionary dict;
    RegularExpression::Verbose = true;
    
    Y_CHECK(dict.insert("INT","[:digit:]+"));
    if(argc>1)
    {
        const string      rx = argv[1];
        auto_ptr<Pattern> p  = RegularExpression::Compile_(rx,&dict); Y_CHECK(p.is_valid());
        auto_ptr<Pattern> q  = RegularExpression::Compile(rx,&dict);  Y_CHECK(q.is_valid());
        const size_t written = p->save_to("regexp.bin");
        Y_CHECK( ios::local_file::length_of("regexp.bin") == written );
        const size_t written2 = q->save_to("regexp2.bin");
        Y_CHECK( ios::local_file::length_of("regexp2.bin") == written2 );
        
        p->graphViz("regexp.dot");
        q->graphViz("regexp2.dot");
        
        Y_CHECK(p->univocal()==q->univocal());
        Y_CHECK(p->feeble()==q->feeble());

        
        std::cerr << "strong   : " << q->strong() << std::endl;
        std::cerr << "univocal : " << q->univocal() << std::endl;
        std::cerr << "entropy  : " << Information::Entropy::Of(*q) << std::endl;
        
    }
    
}
Y_UTEST_DONE()
