#include "y/jive/pattern/basic/range.hpp"
#include "y/information/entropy.hpp"

namespace upsylon {

    namespace Jive
    {
        Y_PATTERN_CLID(Range);

        Range:: ~Range() throw() {}

        Range::  Range(const uint8_t lo, const uint8_t up) throw() :
        Basic(UUID), lower(lo), upper(up)
        {
            assert(lo<=up);
            Y_PATTERN_SELF(Range);
        }

        Range::  Range(const Range &_) throw() :
        Basic(_),
        lower(_.lower),
        upper(_.upper)
        {
            assert(UUID==uuid);
            Y_PATTERN_SELF(Range);
        }

        Pattern *Range:: clone() const
        {
            return new Range(*this);
        }

        Range     * Range:: Create(const uint8_t lo, const uint8_t up)
        {
            if(lo<=up)
                return new Range(lo,up);
            else
                return new Range(up,lo);
        }


        const char *Range:: className() const throw()
        {
            return CLID;
        }

        size_t Range:: serialize(ios::ostream &fp) const
        {
            const size_t ans = id(fp);
            fp.write(lower);
            fp.write(upper);
            return ans + 2;
        }

        bool Range:: isValid(const uint8_t c) const throw()
        {
            return lower<=c && c <= upper;
        }

        void Range:: vizCore(ios::ostream &fp) const
        {
            endl(fp << "[label=\"" << cchars::printable[lower] << '-' << cchars::printable[upper] <<  "\",shape=box]");
        }

        void Range:: start(FirstChars &fc) const
        {
            assert(fc.size()==0);
            const unsigned Upper = upper;
            for(unsigned i=lower;i<=Upper;++i)
            {
                fc.insert( uint8_t(i) );
            }
        }

        void Range:: update(Entropy &E) const throw()
        {
            ++E[lower];
            ++E[upper];
        }

        std::ostream & operator<<(std::ostream &os, const Range &p)
        {
            os << '[' << cchars::visible[p.lower] << '-' << cchars::visible[p.upper] << ']';
            return os;
        }
        
        void Range::express(ios::ostream &fp) const
        {
            Code(Code(fp,lower) << '-',upper);
        }

    }
}
