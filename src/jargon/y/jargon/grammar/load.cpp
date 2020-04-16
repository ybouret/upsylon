
#include "y/jargon/grammar.hpp"
#include "y/type/aliasing.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace Jargon
    {
        
       
        
        XNode * Grammar:: loadTree(Context      &where,
                                   ios::istream &input) const
        {
            static const char fn[] = "loadTree";
            static const char ax[] = "Axiom Label";
            int              &read = aliasing::_(where.column);
            
            //------------------------------------------------------------------
            //
            // read the axiom label
            //
            //------------------------------------------------------------------
            size_t        nr = 0;
            const  string id = string::read(input,nr,ax); read+=nr;
           
            
            //------------------------------------------------------------------
            //
            // find the associatied Axiom
            //
            //------------------------------------------------------------------
            const Dogma  *dogma  = axioms.search_by(id);
            if(!dogma) throw exception("%s.%s(unknown %s <%s>) in %s", **title,fn, ax, *id, **(where.tag) );
            const Axiom &axiom = **dogma;
            
            //------------------------------------------------------------------
            //
            // read a lexeme or not ?
            //
            //------------------------------------------------------------------
            auto_ptr<Lexeme> unit = NULL;
            char             C    = 0;
            if( !input.query(C) ) throw exception("%s.%s(missing marker after <%s>) in %s",**title,fn,*id,**(where.tag));
            ++read;
            switch(C)
            {
                case XNode:: BranchMark:
                    // no lexeme!
                    break;
                    
                case XNode:: LexemeMark: {
                    unit      = new Lexeme(where,axiom.label);
                    size_t nu = 0;
                    if( !input.query_upack(nu,nr)) throw exception("%s.%s(missing #chars for <%s>) in %s",**title,fn,*id,**(where.tag));
                    for(size_t i=1;i<=nu;++i)
                    {
                        if( !input.query(C)) throw exception("%s.%s(missing char %u/%u of <%s>)",**title,fn,unsigned(i), unsigned(nu), **(axiom.label) );
                        ++read;
                        unit->append(uint8_t(C));
                    }
                } break;
                    
                default:
                    throw exception("%s.%s(invalid marker '%s' after <%s>) in %s",**title,fn,cchars::encoded[uint8_t(C)],*id,**(where.tag));
            }
            
            //------------------------------------------------------------------
            //
            // create the xnode
            //
            //------------------------------------------------------------------
            auto_ptr<XNode> xnode = XNode::Create(axiom, unit.yield() );
            
            //------------------------------------------------------------------
            //
            // read children
            //
            //------------------------------------------------------------------
            size_t nch = 0;
            if( !input.query_upack(nch,nr)) throw exception("%s.%s(missing #chars for <%s>) in %s",**title,fn,*id,**(where.tag));
            read += nr;
            
            while(nch-- > 0 )
            {
                xnode->children.push_back( loadTree(where,input) );
            }
            
            return xnode.yield();
            

        }
        
    }
    
}
