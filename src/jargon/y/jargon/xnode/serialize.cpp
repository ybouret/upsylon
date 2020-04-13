
#include "y/jargon/axiom/all.hpp"
#include "y/ios/ostream.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        
        
        static inline size_t __XNodeDeclare(ios::ostream &fp,
                                            const XNode  &xnode,
                                            const uint8_t xmark)
        {
            fp.write(xmark);
            return 1 + fp.write_block( *(xnode.dogma->label) );
        }
        
        size_t XNode:: serialize(ios::ostream &fp) const
        {
            switch(genre)
            {
                    
                case IsTerminal: {
                    assert(Terminal::UUID==dogma->uuid);
                    assert(lexeme.is_valid());
                    const size_t count = __XNodeDeclare(fp,*this,TerminalMark);
                    return count + lexeme->serialize(fp);
                }
                    
                case IsInternal: {
                    assert(lexeme.is_empty());
                    size_t count = __XNodeDeclare(fp,*this,InternalMark);
                    count += fp.write_upack(children.size);
                    for(const XNode *xnode = children.head;xnode;xnode=xnode->next)
                    {
                        count += xnode->serialize(fp);
                    }
                    return count;
                }
            }
            throw exception("XNode::serialize(corrupted!!!)");
        }
        
        const char * XNode:: className() const throw()
        {
            return "XNode";
        }
        
        
    }
}

