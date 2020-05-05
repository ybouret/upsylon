#include "y/spade/types.hpp"
#include "y/randomized/bits.hpp"
#include "y/string.hpp"

namespace upsylon {
    
    namespace Spade {
        
        const Coord1D Coord::__Zero[4] = { 0,0,0,0 };
        const Coord1D Coord::__Ones[4] = { 1,1,1,1 };

        Coord1D Coord::Natural1D(const Coord1D m, randomized::bits &ran ) throw()
        {
            return ran.range<Coord1D>(0, abs_of(m) );
        }
        
        Coord1D Coord::Integer1D(const Coord1D m, randomized::bits &ran ) throw()
        {
            return ran.range<Coord1D>(-m,m);
        }
        
        static inline void disp1(std::ostream &os, const Coord1D c,const size_t w)
        {
            const string s = vformat("%ld", static_cast<long>(c));
            for(size_t i=s.size();i<=w;++i) os << ' ';
            os << s;
        }
        
        size_t Coord::DispWidth = 4;
        std::ostream & Coord::Disp(std::ostream &os,const Coord1D *c,const unsigned dim)
        {
            assert(c);
            assert(dim>=1);
            assert(dim<=3);
            os << '[';
            disp1(os,c[0],DispWidth);
            for(unsigned i=1;i<dim;++i)
            {
                disp1(os << ' ',c[i],DispWidth);
            }
            os << ']';
            return os;
        }

    }
    
}
