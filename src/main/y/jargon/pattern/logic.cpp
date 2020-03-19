
#include "y/jargon/pattern/logic.hpp"
#include "y/ios/ostream.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Logic:: ~Logic() throw()
        {
        }
        
        Logic:: Logic(const uint32_t u) throw() :
        Pattern(u)
        {
        }
        
        Logic::Logic(const Logic &other) :
        Pattern(other),
        Operands(other)
        {
            
        }
        
        size_t Logic:: serialize(ios::ostream &fp) const
        {
            size_t n = emitUUID(fp);
            {
                size_t ns = 0;
                fp.emit_upack(size,&ns);
                n += ns;
            }
            for(const Pattern *op = head; op; op=op->next)
            {
                n += op->serialize(fp);
            }
            return n;
        }
        
        void Logic:: viz(ios::ostream &fp) const
        {
            for(const Pattern *op = head; op; op=op->next)
            {
                op->vizSave(fp);
                endl(vizJoin(fp,op));
            }
        }
        
       
        
    }
    
}

