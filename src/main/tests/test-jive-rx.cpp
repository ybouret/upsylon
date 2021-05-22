
#include "y/jive/pattern/dictionary.hpp"
#include "y/jive/pattern/all.hpp"
#include "y/jive/pattern/posix.hpp"
#include "y/jive/common-regexp.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"
#include "y/ios/icstream.hpp"
#include "y/fs/vfs.hpp"

using namespace upsylon;
using namespace Jive;

Y_UTEST(jive_rx)
{
    
    if(argc<=1)
    {
#define COM_RX(NAME) do {\
std::cerr << "compiling " << #NAME << std::endl;  \
const Motif  m  = RegExp( RegExpFor::NAME,NULL); \
m->save_to(  #NAME ".bin" );\
m->graphViz( #NAME ".dot" );\
std::cerr << "|_feeble: " << m->feeble() << std::endl;\
} while(false)
        
        
        COM_RX(Identifier);
        COM_RX(Unsigned);
        COM_RX(Integer);
        COM_RX(Double);
        COM_RX(Float);
    }
    
    Dictionary        dict;
    //RegExpVerbose() = true;
    if(!dict.use("DIGIT","[:digit:]"))
    {
        throw exception("couldn't insert DIGIT");
    }
    if(!dict.use("INT","{DIGIT}+"))
    {
        throw exception("couldn't insert INT");
    }
    
    if(argc>1)
    {
        std::cerr << "---> compiling" << std::endl;
        auto_ptr<Pattern> p = RegExp(argv[1],&dict);
        std::cerr << "---> save to binary" << std::endl;
        p->save_to("rx.bin");
        std::cerr << "---> save to graphViz" << std::endl;
        p->graphViz("rx.dot");
        std::cerr << "---> build expression" << std::endl;
        const string expr = p->toRegExp();
        std::cerr << "expr='" << expr << "'" << std::endl;
        {
            std::cerr << "---> recompile expression" << std::endl;
            auto_ptr<Pattern> q = RegExp(expr,0);
            q->save_to("rx2.bin");
            q->graphViz("rx2.dot");
            Y_CHECK(*p==*q);
            std::cerr << "---> ignore case" << std::endl;
            auto_ptr<Pattern> r = Pattern::IgnoreCase(p.content());
            r->save_to("ign.bin");
            r->graphViz("ign.dot");
        }
        
        if(argc>2)
        {
            const string  fileName = argv[2];
            const string  baseName = vfs::get_base_name(fileName);
            ios::icstream fp(fileName);
            string        line;
            unsigned      iline=1;
            while( (std::cerr << "> ").flush(),fp.gets(line) )
            {
                const string dataName = baseName + vformat(":%u: ",iline);
                Source       source( Module::OpenData(dataName,line) );
                p->test(source);
                ++iline;
            }
            
        }
    }
    
}
Y_UTEST_DONE()

