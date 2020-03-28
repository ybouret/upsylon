#include "y/jargon/xcache.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"

using namespace upsylon;
using namespace Jargon;

namespace {
    
    
    Lexeme *createLexeme(Cache         &cache,
                         const Context &context,
                         const Tag     &tag
                         )
    {
        auto_ptr<Lexeme> lexeme = new Lexeme(cache,context,tag);
        for(size_t i=1+alea.leq(8);i>0;--i)
        {
            lexeme->append( alea.range('a','z'));
        }
        return lexeme.yield();
    }
    
    XNode *createXNode(Cache           &cache,
                       const Context   &context,
                       const Tag       &tag,
                       XCache          &xcache)
    {
        const Dogma intr = new Terminal("intr");
        const Dogma term = new Terminal("term");
        
        switch(alea.range<int>(0,2))
        {
            case 0: return xcache.query();
            case 1: return xcache.query(intr,NULL);
            default: break;
        }
        return xcache.query( term,createLexeme(cache, context, tag) );
        
    }
    
    
}

Y_UTEST(xnode)
{
    const Tag       label   = Tags::Make("xnode");
    const Context   context(label);
    Cache           cache;
    XCache          xcache("dull");
    
    const Dogma a    = new Inactive("toto");
    
    for(size_t iter=1;iter<=4;++iter)
    {
        auto_ptr<XNode> xnode = xcache.query(a,NULL); Y_ASSERT(xnode->isInternal());
        size_t          count = 1;
        // first layer
        for(size_t n=1+alea.leq(10);n>0;--n)
        {
            xnode->children().push_back( createXNode(cache, context, label,xcache) );
            ++count;
        }
        
        //second layer
        for(XNode *xn=xnode->children().head;xn;xn=xn->next)
        {
            if( xn->isInternal() )
            {
                for(size_t i=1+alea.leq(3);i>0;--i)
                {
                    xn->children().push_back(createXNode(cache, context, label,xcache));
                    ++count;
                }
            }
        }
        
        if(1==(iter%2))
        {
            if(iter<=1)
            {
                xnode->graphViz("xnode.dot",true);
            }
            xcache.store( xnode.yield() );
            
            //xcache.push_back( xnode.yield() );
            std::cerr << "#cache=" << xcache.size << "/" << count << std::endl;
            if(1==iter)
            {
                assert(xcache.size==count);
            }
        }
        else
        {
            std::cerr << "#count=" << count << std::endl;
        }
    }
    
    
}
Y_UTEST_DONE()

