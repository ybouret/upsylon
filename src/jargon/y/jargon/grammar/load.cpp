
#include "y/jargon/grammar.hpp"
#include "y/exception.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {

    namespace Jargon
    {
        
        const Axiom & Grammar:: loadAxiom(ios::istream &fp, int &r, const char *which) const
        {
            static const char fn[] = "loadAxiom";
            assert(which);
            size_t        shift  = 0;
            const string  id     = string::read(fp, shift,which); r += shift;
            const Dogma  *dogma  = axioms.search_by(id);
            if(!dogma) throw exception("%s.%s(unknown %s <%s>)", **title,fn, which, *id);
            return **dogma;
        }
        
        XNode * Grammar:: loadTree(Context      &where,
                                   Cache        &cache,
                                   ios::istream &input) const
        {
            static const char fn[] = "loadTree";
            
            char    C = 0;
            int    &r = aliasing::_(where.column);

            if(!input.query(C)) throw exception("%s.%s(unexpected end of stream)", **title, fn);
            
            ++r;
            switch(C)
            {
                case XNode::InactiveMark: return xcache.query();
                    
                case XNode::TerminalMark:
                {
                    // get the axiom
                    const Axiom     &axiom = loadAxiom(input, r, "Terminal Label");
                    
                    // get the terminal
                    if(Terminal::UUID!=axiom.uuid) throw exception("%s.%s( <%s> is not a Terminal!)",**title,fn,**(axiom.label) );
                    const Terminal &t = axiom.as<Terminal>();
                    
                    
                    // read the lexeme
                    auto_ptr<Lexeme> unit = new Lexeme(cache,where,t.label);
                    size_t           nch  = 0;
                    {
                        size_t          shift = 0;
                        if(!input.query_upack(nch,shift))
                            throw exception("%s.%s( cannot read #chars for <%s>)",**title,fn,**(axiom.label) );
                        r+=shift;
                    }
                    
                    for(size_t i=1;i<=nch;++i)
                    {
                        if( !input.query(C))
                        {
                            throw exception("%s.%s(missing char %u/%u of <%s>)",**title,fn,unsigned(i), unsigned(nch), **(axiom.label) );
                        }
                        ++r;
                        unit->append(uint8_t(C));
                    }
                    
                    return xcache.query(t, unit.yield() );
                    
                }
                case XNode::InternalMark:
                {
                    // load the axiom
                    const Axiom &axiom = loadAxiom(input, r, "Internal Label");
                    
                    
                    // get the aggregate
                    if(Aggregate::UUID!=axiom.uuid) throw exception("%s.%s( <%s> is not an Aggregrate!)",**title,fn,**(axiom.label) );
                    
                    const Aggregate &internal = axiom.as<Aggregate>();
                    if(Aggregate::Design == internal.feature) throw exception("%s.%s( <%s> is a Design Aggregrate!)",**title,fn,**(axiom.label) );
                   
                    // get the xnode
                    auto_ptr<XNode> xnode = xcache.query(internal);
                    
                    // read the children
                    size_t          nch   = 0;
                    {
                        size_t          shift = 0;
                        if(!input.query_upack(nch,shift))
                            throw exception("%s.%s( cannot read #children for <%s>)",**title,fn,**(axiom.label) );
                        r+=shift;
                    }

                    while(nch-- > 0)
                    {
                        xnode->children().push_back( loadTree(where,cache,input) );
                    }
                    
                    return xnode.yield();
                }
                    
                default: break;
            }
            throw exception("%s.%s(unknown mark '%s')", **title, fn, cchars::encoded[ uint8_t(C) ]);
        }
    }
    
}
