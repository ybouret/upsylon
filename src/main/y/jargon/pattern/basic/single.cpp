#include "y/jargon/pattern/basic/single.hpp"
#include "y/ios/ostream.hpp"
#include "y/code/utils.hpp"
#include "y/jargon/first-chars.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Y_PATTERN_CLID(Single);
        
        Single:: Single(const uint8_t c) throw() :
        Basic(UUID),
        code(c)
        {
            Y_PATTERN_SELF(Single);
        }
        
        Single:: ~Single() throw()
        {
        }
        
        Pattern * Single:: Create(const uint8_t c)
        {
            return new Single(c);
        }
        
        Pattern * Single:: clone() const
        {
            return new Single(code);
        }
        
        size_t Single::serialize(ios::ostream &fp) const
        {
            const size_t n = fp.write_nbo(UUID);
            fp << code;
            return n+1;
        }
        
        bool Single:: alike(const Pattern *p) const throw()
        {
            return ( (UUID==p->uuid) && ( p->as<Single>()->code == code) );
        }
        
        bool Single:: good(const uint8_t ch) const throw()
        {
            return ch==code;
        }
        
        bool Single:: univocal() const throw() { return true; }
        
        void Single:: vizCore(ios::ostream &fp) const
        {
            endl(fp << " [label=\"" << cchars::printable[code] << "\",shape=square,style=" << vizStyle() << "]");
        }
        
        void Single:: adjoin(FirstChars &fc) const
        {
            fc.insert( code );
        }

    }
    
}
