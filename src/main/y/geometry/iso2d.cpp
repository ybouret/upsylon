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

    }

}

namespace upsylon
{
    namespace geometry
    {


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

    }

}

namespace upsylon
{
    namespace geometry
    {

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

    }

}

namespace upsylon
{
    namespace geometry
    {

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

    }

}

namespace upsylon
{
    namespace geometry
    {

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

    }

}

namespace upsylon
{
    namespace geometry
    {

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

namespace upsylon
{
    namespace geometry
    {

        //
        iso2d:: point:: point(const shared_point &p) throw() : shared_point(p), next(0), prev(0) {}

        iso2d:: point:: ~point() throw() {}

    }

}

namespace upsylon
{
    namespace geometry
    {
        //
        iso2d:: line:: line() throw() : object(), point::list(), next(0), prev(0) {}

        iso2d:: line:: ~line() throw() {}

        void iso2d:: line:: compile_to( sequence<vertex> &vertices ) const
        {
            vertices.free();
            vertices.ensure(size);
            for(const point *p=head;p;p=p->next)
            {
                vertices.push_front( (*p)->vtx );
            }
        }

    }

}

namespace upsylon
{
    namespace geometry
    {
        //
        iso2d:: lines:: ~lines() throw()
        {
        }

        iso2d:: lines:: lines() throw() : line::list()
        {
        }

        void iso2d:: lines:: connect(const segment::list &segments)
        {
            clear();
            // loop over all segments
            for(const segment *seg=segments.head;seg;seg=seg->next)
            {
                // find if one end of the segment matches a line
                const shared_point &a = seg->a;
                const shared_point &b = seg->b;
                bool found = false;
                for(line *l=head;l;l=l->next)
                {
                    assert(l->size>0);
                    // check growing@head
                    {
                        const shared_point &p = *(l->head);
                        if(a==p)
                        {
                            found = true;
                            l->push_front( new point(b) );
                            break;
                        }
                        if(b==p)
                        {
                            found = true;
                            l->push_front( new point(a) );
                            break;
                        }
                    }

                    // check growing@tail
                    {
                        const shared_point &p = *(l->tail);
                        if(a==p)
                        {
                            found = true;
                            l->push_back( new point(b) );
                            break;
                        }
                        if(b==p)
                        {
                            found = true;
                            l->push_back( new point(a) );
                            break;
                        }
                    }
                }

                // act accordingly
                if(found)
                {
                    // try to merge
                    lines temp;
                    while(size)
                    {
                        line *l      = pop_front();
                        bool  merged = false;
                        const shared_point &l_head = *(l->head);
                        const shared_point &l_tail = *(l->tail);
                        for(line *k=head;k;k=k->next)
                        {
                            const shared_point &k_head = *(k->head);
                            const shared_point &k_tail = *(k->tail);

                            if(k_tail==l_head)
                            {
                                delete l->pop_front();
                                while(l->size)
                                {
                                    k->push_back(l->pop_front());
                                }
                                merged = true;
                                break;
                            }
                            else if(k_head==l_tail)
                            {
                                delete l->pop_back();
                                while(l->size)
                                {
                                    k->push_front(l->pop_back());
                                }
                                merged = true;
                                break;
                            }
                            else if(k_tail==l_tail)
                            {
                                delete l->pop_back();
                                while(l->size)
                                {
                                    k->push_back(l->pop_back());
                                }
                                merged = true;
                                break;
                            }
                            else if(k_head==l_head)
                            {
                                delete l->pop_front();
                                while(l->size)
                                {
                                    k->push_front(l->pop_front());
                                }
                                merged = true;
                                break;
                            }

                        }

                        if(merged)
                        {
                            delete l;
                        }
                        else
                        {
                            temp.push_back(l);
                        }
                    }
                    swap_with(temp);
                }
                else
                {
                    // create a new line
                    line *l = new line();
                    push_back(l);
                    l->push_back( new point(a) );
                    l->push_back( new point(b) );
                }

            }
        }
    }

}



