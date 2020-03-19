
#include "y/jargon/pattern/basic/any1.hpp"
#include "y/jargon/pattern/basic/single.hpp"
#include "y/jargon/pattern/basic/range.hpp"

#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"

using namespace upsylon;
using namespace Jargon;

namespace {
    
    void doTest(const Pattern &p,
                Source &source,
                Token  &content)
    {
        std::cerr << "testing <" << p.className() << ">" << std::endl;
        p.test(source, content);
        source.unget(content);
        std::cerr << "/done" << std::endl;
    }
    
}

Y_UTEST(jargon_pattern)
{
    Char::Cache cache  = new Char::Pool();
    Module     *module = (argc>1) ? Module::OpenFile(cache,argv[1]) : Module::OpenData(cache,"NULL", 0,0);
    Source      source(cache,module);
    Token       content(cache);
    
    auto_ptr<Pattern> p = NULL;
    
    p = Any1::Create();           doTest(*p,source,content);
    p = Single::Create('a');      doTest(*p,source,content);
    p = Range::Create('a', 'z');  doTest(*p,source,content);
    
}
Y_UTEST_DONE()

