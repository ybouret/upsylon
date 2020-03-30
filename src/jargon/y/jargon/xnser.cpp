
#include "y/jargon/axiom/terminal.hpp"
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
                case IsInactive:
                    assert(Inactive::UUID==dogma->uuid);
                    fp.write(InactiveMark);
                    return 1;
                    
                case IsTerminal: {
                    assert(Terminal::UUID==dogma->uuid);
                    const size_t count = __XNodeDeclare(fp,*this,TerminalMark);
                    return count + lexeme().serialize(fp);
                }
                    
                case IsInternal: {
                    size_t count = __XNodeDeclare(fp,*this,InternalMark);
                    count       += fp.write_nbo(dogma->uuid);
                    for(const XNode *xnode = children().head;xnode;xnode=xnode->next)
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

