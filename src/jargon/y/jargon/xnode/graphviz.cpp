
#include "y/jargon/axiom.hpp"
#include "y/ios/ostream.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        static inline void emitLabel(ios::ostream &fp, const Tag &label)
        {
            const char *p = **label;
            for(size_t i=label->size();i>0;--i,++p)
            {
                fp << cchars::printable[ uint8_t(*p) ];
            }
        }
        
        void XNode:: vizCore( ios::ostream &fp ) const
        {
            //__________________________________________________________________
            //
            // label/shape/style
            //
            //------------------------------------------------------------------
            fp << "[" << dogma->vizual() << ",label=\"";
            emitLabel(fp, dogma->label);
            if( lexeme.is_valid() && lexeme->size>0 )
            {
                fp << '=' << '\'';
                for(const Char *ch=lexeme->head;ch;ch=ch->next)
                {
                    fp << cchars::printable[ch->code];
                }
                fp << '\'';
            }
            
            
            endl( fp << "\"]");
            
            const XList &chld = children;
            const bool   show = chld.size>1;
            unsigned i=0;
            for(const XNode *node=chld.head;node;node=node->next)
            {
                node->vizSave(fp);
                vizJoin(fp,node);
                if(show)
                {
                    fp(" [label=\"%u\"]",++i);
                }
                endl(fp);
            }
            
            
        }
        
    }
}

