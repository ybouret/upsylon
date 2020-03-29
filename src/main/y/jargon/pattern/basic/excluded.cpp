
#include "y/jargon/pattern/basic/excluded.hpp"
#include "y/ios/ostream.hpp"
#include "y/code/utils.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Y_PATTERN_CLID(Excluded);
        
        Excluded:: Excluded(const uint8_t c) throw() :
        Basic(UUID),
        code(c)
        {
            Y_PATTERN_SELF(Excluded);
        }
        
        Excluded:: ~Excluded() throw()
        {
        }
        
        Pattern * Excluded:: Create(const uint8_t c)
        {
            return new Excluded(c);
        }
        
        Pattern * Excluded:: clone() const
        {
            return new Excluded(code);
        }
        
        size_t Excluded::serialize(ios::ostream &fp) const
        {
            const size_t n = fp.write_nbo(UUID);
            fp << code;
            return n+1;
        }
        
        bool Excluded:: alike(const Pattern *p) const throw()
        {
            return ( (UUID==p->uuid) && ( p->as<Excluded>()->code == code) );
        }
        
        bool Excluded:: good(const uint8_t ch) const throw()
        {
            return ch!=code;
        }
        
        bool Excluded:: univocal() const throw() { return false; }
        
        void Excluded:: vizCore(ios::ostream &fp) const
        {
            endl(fp << " [label=\"" << cchars::printable[code] << "\",shape=Msquare,style=" << vizStyle() << "]");
        }
        
    }
    
}
