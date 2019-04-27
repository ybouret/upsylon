#include "y/geometry/iso2d.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace geometry
    {

        unsigned Iso2d:: sign_flag( const double value ) throw()
        {
            if(value<0)      return sign_negative;
            else if(value>0) return sign_positive;
            else             return sign_zero;
        }

        Iso2d::Vertex   Iso2d::zfind(const Vertex &pa, const double va,
                                     const Vertex &pb, const double vb) throw()
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
            return Vertex( pa.x + lam * (pb.x-pa.x), pa.y + lam * (pb.y-pa.y) );
        }

    }

}

namespace upsylon
{
    namespace geometry
    {


        //
        Iso2d:: Coordinate::  Coordinate(const unit_t ii, const unit_t jj, const unsigned qq) throw() : i(ii), j(jj), q(qq) { assert(q<=1); }

        Iso2d:: Coordinate:: ~Coordinate() throw()
        {
        }

        Iso2d:: Coordinate:: Coordinate(const Coordinate &other) throw() :
        i(other.i), j(other.j), q(other.q) {}

        bool operator==( const Iso2d::Coordinate &lhs, const Iso2d::Coordinate &rhs) throw()
        {
            return (lhs.i==rhs.i) && (lhs.j==rhs.j) && (lhs.q==rhs.q);
        }


        int Iso2d::Coordinate:: compare(const Coordinate &lhs, const Coordinate &rhs) throw()
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

        void Iso2d:: Coordinate:: run( hashing::function &H ) const throw()
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
        Iso2d:: EdgeLabel:: EdgeLabel( const Coordinate &only ) throw() : lower(only), upper(only) {}

        Iso2d:: EdgeLabel:: EdgeLabel( const Coordinate &a, const Coordinate &b) throw() : lower(a), upper(b)
        {
            if(Coordinate::compare(lower,upper)>0) mswap( (void*)&lower, (void*)&upper, sizeof(Coordinate) );
        }

        Iso2d:: EdgeLabel:: EdgeLabel(const EdgeLabel &other) throw() : lower(other.lower), upper(other.upper) {}


        void Iso2d:: EdgeLabel:: run( hashing::function &H ) const throw()
        {
            lower.run(H);
            upper.run(H);
        }

        bool operator==(const Iso2d::EdgeLabel &lhs, const Iso2d::EdgeLabel &rhs) throw()
        {
            return (lhs.lower==rhs.lower) && (lhs.upper==rhs.upper);
        }

        //
        Iso2d:: EdgeLabel:: Hasher:: Hasher() throw() : H() {}

        Iso2d:: EdgeLabel:: Hasher:: ~Hasher() throw()  {}

        size_t Iso2d:: EdgeLabel:: Hasher:: operator()( const EdgeLabel &id) throw()
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
        Iso2d:: UniquePoint :: UniquePoint(const EdgeLabel  &id,
                                           const Vertex     &v) throw() :
        tag(id),
        vtx(v)
        {
            assert(tag==id);
        }

        Iso2d:: UniquePoint :: ~UniquePoint() throw()
        {
        }

        const Iso2d::EdgeLabel & Iso2d::UniquePoint:: key() const throw() { return tag; }

    }

}

namespace upsylon
{
    namespace geometry
    {

        //
        Iso2d:: Segment:: Segment(UniquePoint       *pa, UniquePoint     *pb) throw() :
        next(0), prev(0), a(pa), b(pb)
        {
            assert(a->refcount()>1); assert( object::is_block(pa) );
            assert(b->refcount()>1); assert( object::is_block(pb) );
        }

        Iso2d:: Segment:: ~Segment() throw()
        {
        }

    }

}

namespace upsylon
{
    namespace geometry
    {

        //
        Iso2d:: SharedPoints:: SharedPoints() throw(): segments() {}

        Iso2d:: SharedPoints:: ~SharedPoints() throw() {}


        Iso2d::UniquePoint * Iso2d::SharedPoints:: operator()(const Coordinate &c,
                                                               const Vertex     &fallback)
        {
            const EdgeLabel tag(c);
            SharedPoint     *psp = search(tag);
            if(psp)
            {
                return & **psp;
            }
            else
            {
                UniquePoint       *up = new UniquePoint(tag,fallback);
                const SharedPoint  sp = up; assert(tag==sp->tag);
                if(!insert(sp)) throw exception("unexpected multiple Iso2d::vertex @(%d,%d,+%u)!",int(c.i), int(c.j), c.q);
                return up;
            }
        }

        Iso2d::UniquePoint * Iso2d::SharedPoints:: operator()(const Coordinate &c0, const Vertex &p0, const double v0,
                                                                const Coordinate &c1, const Vertex &p1, const double v1)
        {
            const EdgeLabel tag(c0,c1);
            SharedPoint    *psp = search(tag);
            if( psp )
            {
                return & **psp;
            }
            else
            {
                const Vertex       vv = zfind(p0, v0, p1, v1);
                UniquePoint       *up = new UniquePoint(tag,vv);
                const SharedPoint  sp = up;
                if(!insert(sp)) throw exception("unexpected multiple Iso2d::vertex @(%d,%d,+%u)-(%d,%d,+%u)!",
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
        Iso2d:: Point:: Point(const SharedPoint &p) throw() : SharedPoint(p), next(0), prev(0) {}

        Iso2d:: Point:: ~Point() throw() {}

    }

}

namespace upsylon
{
    namespace geometry
    {
        //
        Iso2d:: Line:: Line() throw() : object(), Points(), next(0), prev(0) {}

        Iso2d:: Line:: ~Line() throw() {}

        void Iso2d:: Line:: compile_to( sequence<Vertex> &vertices ) const
        {
            vertices.free();
            vertices.ensure(size);
            for(const Point *p=head;p;p=p->next)
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
        Iso2d:: Lines:: ~Lines() throw()
        {
        }

        Iso2d:: Lines:: Lines() throw() : Line::list()
        {
        }

        void Iso2d:: Lines:: connect(const Segments &segments)
        {
            release();
            // loop over all segments
            for(const Segment *seg=segments.head;seg;seg=seg->next)
            {
                // find if one end of the segment matches a line
                const SharedPoint &a = seg->a;
                const SharedPoint &b = seg->b;
                bool found = false;
                for(Line *l=head;l;l=l->next)
                {
                    assert(l->size>0);
                    // check growing@head
                    {
                        const SharedPoint &p = *(l->head);
                        if(a==p)
                        {
                            found = true;
                            l->push_front( new Point(b) );
                            break;
                        }
                        if(b==p)
                        {
                            found = true;
                            l->push_front( new Point(a) );
                            break;
                        }
                    }

                    // check growing@tail
                    {
                        const SharedPoint &p = *(l->tail);
                        if(a==p)
                        {
                            found = true;
                            l->push_back( new Point(b) );
                            break;
                        }
                        if(b==p)
                        {
                            found = true;
                            l->push_back( new Point(a) );
                            break;
                        }
                    }
                }

                // act accordingly
                if(found)
                {
                    // try to merge
                    Lines temp;
                    while(size)
                    {
                        Line *l      = pop_front();
                        bool  merged = false;
                        const SharedPoint &l_head = *(l->head);
                        const SharedPoint &l_tail = *(l->tail);
                        for(Line *k=head;k;k=k->next)
                        {
                            const SharedPoint &k_head = *(k->head);
                            const SharedPoint &k_tail = *(k->tail);

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
                    Line *l = new Line();
                    push_back(l);
                    l->push_back( new Point(a) );
                    l->push_back( new Point(b) );
                }

            }
        }
    }

}

namespace upsylon
{
    namespace geometry
    {

        Iso2d:: Curve::  Curve() throw() : CurveType() {}
        Iso2d:: Curve:: ~Curve() throw() {}

    }
}

namespace upsylon
{
    namespace geometry
    {

        Iso2d:: Curves::  Curves() throw() : CurvesType() {}
        Iso2d:: Curves:: ~Curves() throw() {}
    }
}

namespace upsylon
{
    namespace geometry
    {
        void Iso2d:: convert(LevelSet     &output,
                             const Levels &input)
        {
            const size_t n = input.size();
            output.free();
            output.ensure(n);
            for(size_t i=1;i<=n;++i)
            {
                const SharedPoints  &db = *input[i];
                Lines                Lines;
                Lines.connect(db.segments);
                Curves *curves_ptr = new Curves();
                {
                    const Curves::Pointer tmp(curves_ptr);
                    output.push_back(tmp);
                }
                Lines.compile_to(*curves_ptr);
            }
        }
    }
}




