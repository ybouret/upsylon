#include "y/geometry/iso2d.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace geometry
    {

        unsigned iso2d:: sign_flag( const double value ) throw()
        {
            if(value<0)      return sign_negative;
            else if(value>0) return sign_positive;
            else             return sign_zero;
        }

        iso2d::vertex   iso2d::zfind(const vertex &pa, const double va,
                                     const vertex &pb, const double vb) throw()
        {
            assert(va*vb<=0);
            double num = -va;
            double den = vb-va;
            if(den<0)
            {
                num = -num;
                den = -den;
            }
            const double lam = num/den;
            return vertex( pa.x + lam * (pb.x-pa.x), pa.y + lam * (pb.y-pa.y) );
        }


        //
        iso2d:: coordinate::  coordinate(const unit_t ii, const unit_t jj, const unsigned qq) throw() : i(ii), j(jj), q(qq) { assert(q<=1); }

        iso2d:: coordinate:: ~coordinate() throw()
        {
        }

        iso2d:: coordinate:: coordinate(const coordinate &other) throw() :
        i(other.i), j(other.j), q(other.q) {}

        bool operator==( const iso2d::coordinate &lhs, const iso2d::coordinate &rhs) throw()
        {
            return (lhs.i==rhs.i) && (lhs.j==rhs.j) && (lhs.q==rhs.q);
        }


        int iso2d::coordinate:: compare(const coordinate &lhs, const coordinate &rhs) throw()
        {
            if(lhs.i<rhs.i)
            {
                return -1;
            }
            else if(rhs.i<lhs.i)
            {
                return 1;
            }
            else
            {
                assert(lhs.i==rhs.i);
                if(lhs.j<rhs.j)
                {
                    return -1;
                }
                else if(rhs.j<lhs.j)
                {
                    return 1;
                }
                else
                {
                    assert(rhs.j==lhs.j);
                    return (lhs.q<rhs.q) ? -1 : ( (rhs.q<lhs.q) ? 1:0 );
                }
            }
        }

        void iso2d:: coordinate:: run( hashing::function &H ) const throw()
        {
            H.run_type(i);
            H.run_type(j);
            H.run_type(q);
        }

        //
        iso2d:: edge_label:: edge_label( const coordinate &only ) throw() : lower(only), upper(only) {}

        iso2d:: edge_label:: edge_label( const coordinate &a, const coordinate &b) throw() : lower(a), upper(b)
        {
            if(coordinate::compare(lower,upper)>0) mswap( (void*)&lower, (void*)&upper, sizeof(coordinate) );
        }

        iso2d:: edge_label:: edge_label(const edge_label &other) throw() : lower(other.lower), upper(other.upper) {}


        void iso2d:: edge_label:: run( hashing::function &H ) const throw()
        {
            lower.run(H);
            upper.run(H);
        }

        bool operator==(const iso2d::edge_label &lhs, const iso2d::edge_label &rhs) throw()
        {
            return (lhs.lower==rhs.lower) && (lhs.upper==rhs.upper);
        }

        //
        iso2d:: edge_label:: hasher:: hasher() throw() : H() {}

        iso2d:: edge_label:: hasher:: ~hasher() throw()  {}

        size_t iso2d:: edge_label:: hasher:: operator()( const edge_label &id) throw()
        {
            H.set();
            id.run(H);
            return H.key<size_t>();
        }

        //
        iso2d:: unique_point :: unique_point(const edge_label &id,
                                             const vertex     &v) throw() :
        tag(id),
        vtx(v)
        {
            assert(tag==id);
        }

        iso2d:: unique_point :: ~unique_point() throw()
        {
        }

        const iso2d::edge_label & iso2d::unique_point:: key() const throw() { return tag; }


        //
        iso2d:: segment:: segment(unique_point       *pa, unique_point     *pb) throw() :
        next(0), prev(0), a(pa), b(pb)
        {
            assert(a->refcount()>1); assert( object::is_block(pa) );
            assert(b->refcount()>1); assert( object::is_block(pb) );
        }

        iso2d:: segment:: ~segment() throw()
        {
        }

        //
        iso2d:: shared_points:: shared_points() throw(): segments() {}

        iso2d:: shared_points:: ~shared_points() throw() {}


        iso2d::unique_point * iso2d::shared_points:: operator()(const coordinate &c,
                                                                 const vertex     &fallback)
        {
            const edge_label tag(c);
            shared_point *psp = search(tag);
            if(psp)
            {
                return & **psp;
            }
            else
            {
                unique_point      *up = new unique_point(tag,fallback);
                const shared_point sp = up; assert(tag==sp->tag);
                if(!insert(sp)) throw exception("unexpected multiple iso2d::vertex @(%d,%d,+%u)!",int(c.i), int(c.j), c.q);
                return up;
            }
        }

        iso2d::unique_point * iso2d::shared_points:: operator()(const coordinate &c0, const vertex &p0, const double v0,
                                                                const coordinate &c1, const vertex &p1, const double v1)
        {
            const edge_label tag(c0,c1);
            shared_point *psp = search(tag);
            if( psp )
            {
                return & **psp;
            }
            else
            {
                const vertex       vv = zfind(p0, v0, p1, v1);
                unique_point      *up = new unique_point(tag,vv);
                const shared_point sp = up;
                if(!insert(sp)) throw exception("unexpected multiple iso2d::vertex @(%d,%d,+%u)-(%d,%d,+%u)!",
                                                int(tag.lower.i), int(tag.lower.j), tag.lower.q,
                                                int(tag.upper.i), int(tag.upper.j), tag.upper.q
                                                );
                return up;
            }
        }


    }

}



