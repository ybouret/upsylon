#include "y/oxide/types.hpp"
#include "y/randomized/bits.hpp"

namespace upsylon
{
    namespace Oxide
    {
        const Coord1D Coord:: __Zero[4] = { 0,0,0,0 };
        const Coord1D Coord:: __Ones[4] = { 1,1,1,1 };

        Coord1D Coord:: Natural1D( const unit_t m, randomized::bits &ran ) throw()
        {
            const unit_t ma = abs_of(m);
            return ran.range<Coord1D>(0,ma);
        }

        Coord1D Coord:: Integer1D( const unit_t m, randomized::bits &ran ) throw()
        {
            const unit_t ma = abs_of(m);
            return ran.range<Coord1D>(-ma,ma);
        }

        Coord1D Coord:: Within1D( const Coord1D a, const Coord1D b, randomized::bits &ran) throw()
        {
            return ran.range<Coord1D>(a,b);
        }

        const char *Coord:: DefaultAxisName(const size_t dim) throw()
        {
            switch(dim)
            {
                case 0: return "x";
                case 1: return "y";
                case 2: return "z";
                default: break;
            }
            return "?";
        }



    }

}

#include "y/string/tokenizer.hpp"
#include "y/string/convert.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Oxide
    {
        static const char Fn[] = "Oxide::Coord::";
        
        static inline bool is_blank( const int C ) throw() { return C == ' ' || C == '\t'; }
        
        void Coord:: Parse(Coord1D *p, const size_t n, const string &args, const char *id)
        {
            assert(p!=NULL);
            assert(n==1||n==2||n==3);
            tokenizer<char> tt(args);
            while( tt.next_with(',') )
            {
                string s( tt.token(), tt.units() );
                *(p++) = string_convert::to<unit_t>( s.clean(is_blank),id );
            }

            if(n!=tt.count())
            {
                if(!id) id = "?";
                throw exception("%s::Parse(bad dim for '%s')", Fn, id);
            }
        }
        
        void Coord:: Parse(Coord1D *p, const size_t n, const char   *text, const char *id)
        {
            const string _(text);
            return Parse(p,n,_,id);
        }

        void Coord:: CheckRanks(const Coord1D *size, const Coord1D *rank, const unsigned dim)
        {
            assert(size);
            assert(rank);
            assert(dim>0);
            assert(dim<=3);
            for(unsigned i=0;i<dim;++i)
            {
                const long s = long(size[i]);
                if(s<0) throw exception("%s(invalid size=%ld in dimension#%u)",Fn,s,i);
                const long r = long(rank[i]);
                if(r<0||r>=s) throw exception("%s(rank#%u=%ld not in [0:%ld]", Fn,i, r, s-1 );
            }
        }

        const char * __fmt_coord( const Coord1D c, const string &fmt, string &tmp )
        {
            tmp = vformat(*fmt,c);
            return *tmp;
        }

        std::ostream & Coord:: Display( std::ostream &os, const Coord1D *addr, const size_t size, const unsigned w)
        {
            assert(addr);
            assert(size>0);
            const string fmt = vformat("%%%uld",w);
            string       tmp;
            os << '[' << __fmt_coord(addr[0],fmt,tmp);
            for(size_t i=1;i<size;++i)
            {
                os << ',' << __fmt_coord(addr[i], fmt, tmp);
            }
            os << ']' << '\'';
            return os;
        }
    }
}

