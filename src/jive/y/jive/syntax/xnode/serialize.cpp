#include "y/jive/syntax/axiom.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace  Syntax
        {
         
            const char Node::CLID[] = "XNode";
            
            const char * Node:: className() const throw()
            {
                return CLID;
            }
            
            static inline size_t emitMaker(ios::ostream &fp,
                                           const uint8_t mk)
            {
                fp.write(mk);
                return 1;
            }
            
            size_t Node:: serialize(ios::ostream &fp) const
            {
                size_t written = axiom.name->serialize(fp);
                switch (kind)
                {
                    case IsTerminal: {
                        written += emitMaker(fp,TerminalMarker);
                        const Lexeme *lx = lexeme();
                        if( lx )
                        {
                            written += fp.write_upack( lx->size );
                            for(const Char *ch=lexeme()->head;ch;ch=ch->next)
                            {
                                fp.write(ch->code);
                                ++written;
                            }
                        }
                        else
                        {
                            fp.write(0);
                            ++written;
                        }
                    } break;
                        
                    case IsInternal: {
                        written += emitMaker(fp,TerminalMarker);
                        written += fp.write_upack( leaves().size );
                        for(const Node *node=leaves().head;node;node=node->next)
                        {
                            written += node->serialize(fp);
                        }
                    } break;
                }
                
                return written;
            }
            
        }
    }
    
}

