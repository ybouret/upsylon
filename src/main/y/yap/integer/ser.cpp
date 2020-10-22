
#include "y/yap/integer.hpp"
#include "y/ios/ostream.hpp"
#include "y/ios/istream.hpp"
#include "y/exception.hpp"

namespace upsylon
{

    namespace yap
    {
        //======================================================================
        //
        // I/O
        //
        //======================================================================
        const char integer:: CLASS_NAME[] = "apz";

        const char *integer:: className() const throw() { return CLASS_NAME; }

        size_t integer::serialize(ios::ostream &fp) const
        {
            switch(s)
            {
                case __zero: fp.write( sign2byte(s) ); return 1;
                default:
                    break;
            }
            fp.write( sign2byte(s) );
            return 1 + n.serialize(fp);
        }

        integer integer::read(ios::istream &fp, size_t &delta, const char *which)
        {
            static const char fn[] = "integer::read";
            assert(which);
            delta=0;
            uint8_t b = 0;
            if( !fp.query( (char&)b ) )
            {
                throw exception("%s(missing sign byte for '%s')",fn,which);
            }
            switch(b)
            {
                case __z: delta=1; return integer();

                case __p: {
                    const natural _ = natural::read(fp, delta, which);
                    ++delta;
                    if(_<=0) throw exception("%s(invalid natural for positive '%s')",fn,which);
                    return integer(__positive,_);
                }
                case __n: {
                    const natural _ = natural::read(fp, delta, which);
                    ++delta;
                    if(_<=0) throw exception("%s(invalid natural for negative '%s')",fn,which);
                    return integer(__negative,_);
                }
                default:
                    break;
            }
            throw exception("%s(invalid sign byte for '%s')",fn,which);
        }

        //
        std::ostream & operator<<( std::ostream &os, const integer &z)
        {
            switch(z.s)
            {
                case __zero:     os << '0'; break;
                case __negative: os << '-' << z.n; break;
                case __positive: os << z.n; break;
            }
            return os;
        }
    }

}

