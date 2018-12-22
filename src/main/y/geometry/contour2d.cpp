#include "y/geometry/contour2d.hpp"

namespace upsylon
{
    namespace geometry
    {
        // fonctions
        unsigned contour2d:: sign_flag( const double value ) throw()
        {
            if(value<0)      return sign_negative;
            else if(value>0) return sign_positive;
            else             return sign_zero;
        }

        // identifier
        contour2d:: identifier:: identifier( const coordinate &c, const offset &d ) throw() :
        coord(c), delta(d)
        {

        }

        contour2d:: identifier:: ~identifier() throw()
        {
        }

        contour2d:: identifier:: identifier( const identifier &other) throw() :
        coord(other.coord), delta(other.delta)
        {
        }

        bool operator==( const contour2d::identifier &lhs, const contour2d::identifier &rhs) throw()
        {
            return (lhs.coord==rhs.coord) && (lhs.delta==rhs.delta);
        }

        bool operator!=( const contour2d::identifier &lhs, const contour2d::identifier &rhs) throw()
        {
            return (lhs.coord!=rhs.coord) || (lhs.delta!=rhs.delta);
        }

        // identifier::hasher
        contour2d:: identifier:: hasher:: hasher() throw() : h()
        {
        }

        contour2d:: identifier:: hasher:: hasher :: ~hasher() throw()
        {
        }

        size_t contour2d::identifier:: hasher:: operator()( const identifier &id ) throw()
        {
            h.set();
            h.run(&id.coord,sizeof(coordinate));
            h.run(&id.delta,sizeof(offset));
            return h.key<size_t>();
        }

        // unique point
        contour2d:: unique_point:: unique_point( const identifier &i, const point &p ) throw() :
        uid(i), pos(p)
        {
        }

        contour2d:: unique_point:: ~unique_point() throw()
        {
        }

        const contour2d:: identifier & contour2d:: unique_point:: key() const throw()
        { return uid; }

        // segment
        contour2d:: segment:: segment( const shared_point &A, const shared_point &B ) throw() :
        next(0), prev(0), a(A), b(B)
        {
            assert(a->uid!=b->uid);
        }

        contour2d:: segment:: ~segment() throw()
        {}

        // segments
        contour2d:: segments:: segments(const size_t id) throw() :
        segment::list_type(),
        indx(id)
        {}

        contour2d:: segments:: ~segments() throw() {}

        const size_t & contour2d:: segments:: key() const throw() { return indx; }

        void contour2d::segments:: build_connected( iso_lines &iso ) const
        {

            iso.clear();
            for(const segment *s = head; s; s=s->next )
            {
                const shared_point &a     = s->a;
                const shared_point &b     = s->b;
                bool                found = false;
                assert(a->uid!=b->uid);
                const identifier    A     = a->uid;
                const identifier    B     = b->uid;

                for(iso_line *line = iso.head; line; line=line->next )
                {
                    const shared_point_node *p = line->head;
                    const identifier         P = p->data->uid;
                    const shared_point_node *q = line->tail;
                    const identifier         Q = q->data->uid;

                    if(A==P)
                    {
                        if(B!=Q)
                        {
                            line->push_front( new shared_point_node(b) );
                            found = true;
                            break;
                        }
                    }

                    if(A==Q)
                    {
                        if(B!=P)
                        {
                            line->push_back( new  shared_point_node(b) );
                            found = true;
                            break;
                        }
                    }

                    if(B==P)
                    {
                        if(A!=Q)
                        {
                            line->push_front( new shared_point_node(a) );
                            found = true;
                            break;
                        }
                    }

                    if(B==Q)
                    {
                        if(A!=P)
                        {
                            line->push_back( new shared_point_node(a) );
                            found = true;
                            break;
                        }
                    }

                }

                if(!found)
                {
                    iso_line *line = new iso_line();
                    iso.push_back(line);
                    line->push_back( new shared_point_node(a) );
                    line->push_back( new shared_point_node(b) );
                }
            }


        }

        
    }
}

