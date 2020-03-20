
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
    
    if(argc>1)
    {
        const string      rx = argv[1];
        auto_ptr<Pattern> p  = RegularExpression::Compile(rx,NULL);
        Y_CHECK(p.is_valid());
        const size_t written = p->save_to("regexp.bin");
        Y_CHECK( ios::local_file::length_of("regexp.bin") == written );
       
        p->graphViz("regexp.dot");
    }
    
}
Y_UTEST_DONE()
