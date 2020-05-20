#include "y/spade/types.hpp"
#include "y/randomized/bits.hpp"
#include "y/string/tokenizer.hpp"
#include "y/exception.hpp"
#include "y/string/convert.hpp"

namespace upsylon {
    
    namespace Spade {
        
        const Coord1D Coord::__Zero[4] = { 0,0,0,0 };
        const Coord1D Coord::__Ones[4] = { 1,1,1,1 };
        const bool    Coord::__OK[4] = { true,  true,  true,  true };
        const bool    Coord::__NO[4] = { false, false, false, false };


        Coord1D Coord::Natural1D(const Coord1D m, randomized::bits &ran ) throw()
        {
            return ran.range<Coord1D>(0, abs_of(m) );
        }
        
        Coord1D Coord::Integer1D(const Coord1D m, randomized::bits &ran ) throw()
        {
            return ran.range<Coord1D>(-m,m);
        }
        
        Coord1D Coord:: Within1D(const Coord1D a, const Coord1D b, randomized::bits &ran ) throw()
        {
            return ran.range<Coord1D>(a,b);
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

#if 0
        static inline string coord1(const Coord1D c)
        {
            string s = vformat("%ld", static_cast<long>(c));
            for(size_t i=s.size();i<=Coord::DispWidth;++i) s << ' ';
            return s;
        }

        string Coord:: ToString( const Coord1D C)
        {
            return coord1(C);
        }

        string Coord:: ToString(const Coord2D C)
        {
            return '[' + coord1(C.x) + ' ' + coord1(C.y) + ']' + '\'';
        }

        string Coord:: ToString(const Coord3D C)
        {
            return '[' + coord1(C.x) + ' ' + coord1(C.y) + ' ' + coord1(C.z) + ']' + '\'';
        }
#endif

        void Coord::Parse(Coord1D       *c,
                          const unsigned dim,
                          const string  &args)
        {
            assert(NULL!=c);
            assert(dim>=1);
            assert(dim<=3);
            tokenizer<char> tkn(args);
            char which[8] = { '#', 'd', 'i', 'm', 0, 0, 0 , 0 };
            
            for(unsigned i=0;i<dim;++i)
            {
                if( !tkn.next_with(':') )
                {
                    throw exception("Spade::Coord('%s' missing #dim=%u')", *args, i);
                }
                string s(tkn.token(),tkn.units());
                s.clean_with(" \t");
                which[4] = '0' + i;
                c[i] = string_convert::to<unit_t>(s,which);
            }
        }

    }
    
}
