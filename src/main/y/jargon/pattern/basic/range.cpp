


#include "y/jargon/pattern/basic/range.hpp"
#include "y/jargon/pattern/basic/single.hpp"
#include "y/ios/ostream.hpp"
#include "y/type/cswap.hpp"
#include "y/code/utils.hpp"
#include "y/jargon/first-chars.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Y_PATTERN_CLID(Range);
        
        Range:: Range(const uint8_t a, const uint8_t b) throw() :
        Basic(UUID),
        lower(a),
        upper(b)
        {
            assert(a<=b);
            Y_PATTERN_SELF(Range);
        }
        
        Range:: ~Range() throw()
        {
        }
        
        Pattern * Range:: Create(const uint8_t a,const uint8_t b)
        {
           if(a<b)
           {
               return new Range(a,b);
           }
           else if(a>b)
           {
               return new Range(b,a);
           }
           else
           {
               assert(a==b);
               return Single::Create(a);
           }
        }
        
        Pattern * Range:: clone() const
        {
            return new Range(lower,upper);
        }
        
        size_t Range::serialize(ios::ostream &fp) const
        {
            const size_t n = fp.write_nbo(uuid);
            fp << lower;
            fp << upper;
            return n+2;
        }
        
        bool Range:: alike(const Pattern *p) const throw()
        {
            if(UUID==p->uuid)
            {
                const Range *rhs = p->as<Range>();
                return (lower==rhs->lower) && (upper==rhs->upper);
            }
            else
            {
                return NULL;
            }
        }
        
        bool Range:: good(const uint8_t ch) const throw()
        {
            return lower<=ch && ch<=upper;
        }
        
        bool Range:: univocal() const throw() { return lower==upper; }
        
        void Range:: vizCore(ios::ostream &fp) const
        {
            endl(fp
                 << " [label=\"["
                 << cchars::printable[lower]
                 << '-'
                 << cchars::printable[upper]
                 << "]\",shape=rect,style=" << vizStyle() << "]");
        }
        
        void Range ::adjoin(FirstChars &fc) const
        {
            const unsigned up = upper;
            for(unsigned i=lower;i<=up;++i)
            {
                fc.insert( uint8_t(i) );
            }
            
        }
        
    }
    
}
