#include "y/jargon/xcache.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"
#include "y/jargon/axiom/terminal.hpp"

using namespace upsylon;
using namespace Jargon;

namespace {
 
    Lexeme *createLexeme(const Tag         &tag,
                         const Context     &ctx,
                         Cache             &cache)
    {
        auto_ptr<Lexeme>  lexeme = new Lexeme(cache,ctx,tag);
        for(size_t i=1+alea.leq(8);i>0;--i)
        {
            lexeme->append( alea.range<char>('a','z') );
        }
        return lexeme.yield();
    }
    
    XNode *createXNode(XCache           &xcache,
                       const Internal   &I,
                       const array<Tag> &tags,
                       const Context    &ctx,
                       Cache            &cache)
    {
        switch(alea.range<int>(1,3))
        {
            case 1: return xcache.query(I);
                
            case 2: return xcache.query();
                
            case 3: {
                const Tag                &tag  = alea.in(tags);
                const arc_ptr<Terminal>  term = new Terminal(tag);
                return xcache.query(*term, createLexeme(tag,ctx,cache) );
            }
                
                
            default: break;
        }
        throw exception("label not implemented");
    }
    
}

Y_UTEST(xnode)
{
   
    const Context ctx       = "xnode";
    const Tag     tagString = Tags::Make( "string" );
    const Tag     tagNumber = Tags::Make( "number" );
    Cache         cache;
    
    vector<Tag>    tags; tags << tagString << tagNumber;
    tags.push_back(tagString);
    tags.push_back(tagNumber);
    
    XCache            xcache( "xcache_dull" );
    arc_ptr<Internal> LIST = new Internal("LIST",0);
    {
        auto_ptr<XNode> root = xcache.query(*LIST); Y_CHECK(root->isInternal());
        
        // first level
        for(size_t i=1+alea.leq(10);i>0;--i)
        {
            root->children().push_back( createXNode(xcache,*LIST,tags,ctx,cache) );
        }
        
        // second level
        for(XNode *xnode = root->children().head; xnode; xnode=xnode->next)
        {
            if( xnode->isInternal() )
            {
                xnode->children().push_back( createXNode(xcache,*LIST,tags,ctx,cache) );
            }
        }
        
        root->graphViz("xnode.dot");
        
        xcache.store( root.yield() );

    }
    
    
}
Y_UTEST_DONE()

