
#include "y/jargon/axiom/all.hpp"
#include "y/ios/ostream.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        
        
        size_t XNode:: serialize(ios::ostream &fp) const
        {
            //------------------------------------------------------------------
            // declare dogma/axiom
            //------------------------------------------------------------------
            size_t count = fp.write_block( *(dogma->label) );
            
            //------------------------------------------------------------------
            // save lexeme
            //------------------------------------------------------------------
            if(lexeme.is_valid())
            {
                fp.write(LexemeMark);
                ++count;
                count += lexeme->serialize(fp);
            }
            else
            {
                fp.write(BranchMark);
                ++count;
            }
     
            //------------------------------------------------------------------
            // save tree
            //------------------------------------------------------------------
            count += fp.write_upack(children.size);
            for(const XNode *xnode = children.head;xnode;xnode=xnode->next)
            {
                count += xnode->serialize(fp);
            }
            return count;
            
            
        }
        
        const char XNode:: CLID[] = "XNode";
        
        const char * XNode:: className() const throw()
        {
            return CLID;
        }
        
        
    }
}

