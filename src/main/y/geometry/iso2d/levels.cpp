
#include "y/geometry/iso2d/levels.hpp"
#include "y/exception.hpp"
#include "y/type/bzset.hpp"

namespace upsylon {

    namespace geometry {

        namespace Iso2D {

            static const char Fn[] = "Iso2D::Level: ";

            Level_:: Level_( const size_t k ) throw() :
            index(k),
            slist(),
            iso()
            {

            }

            Level_:: ~Level_() throw()
            {
                bzset_(index);
            }

            const size_t & Level_:: key() const throw()
            {
                return index;
            }

            void Level_:: buildLines(const bool keep)
            {
                slist.buildLines(iso,keep);
            }

            Point_  *  Level_:: single( const Coordinate &c, const Vertex &v )
            {
                const Edge   e(c);
                Point       *pp = search(e);
                if(pp)
                {

                    return &(**pp);
                }
                else
                {
                    Point_ *p = new Point_(e,v);
                    {
                        const Point tmp = p;
                        if(!insert(tmp)) throw exception("%scan't insert new point!!!",Fn);
                    }
                    return p;
                }
            }



            Point_ * Level_:: couple(const Coordinate &ca, const Vertex &va, const double da,
                                     const Coordinate &cb, const Vertex &vb, const double db)
            {
                assert(ca!=cb);
                assert(da*db<0);
                const Edge e(ca,cb);
                Point *pp = search(e);
                if(pp)
                {
                    return &(**pp);
                }
                else
                {
                    double num = da;
                    double den = da-db;
                    if(den<0)
                    {
                        den=-den;
                        num=-num;
                    }
                    const double lam = (num<=0) ? 0 : ( (num>=den) ? 1 : num/den);
                    const Vertex vz  = va+lam*(vb-va);
                    Point_ *p = new Point_(e,vz);
                    {
                        const Point tmp = p;
                        if(!insert(tmp)) throw exception("%scan't insert new point!!!",Fn);
                    }
                    return p;
                }
            }

            void     Level_:: full(const Coordinate &ca, const Vertex &va,
                                   const Coordinate &cb, const Vertex &vb)
            {
                const Point head = single(ca,va);
                const Point tail = single(cb,vb);
                slist.push_back( new Segment(head,tail) );
            }

            void   Level_:: inter1(const Coordinate &cz, const Vertex &vz,
                                   const Coordinate &ca, const Vertex &va, const double da,
                                   const Coordinate &cb, const Vertex &vb, const double db)
            {
                const Point head = single(cz,vz);
                const Point tail = couple(ca,va,da,cb,vb,db);
                slist.push_back( new Segment(head,tail) );
            }

            void   Level_:: inter2(const Coordinate &cs, const Vertex &vs, const double ds,
                                   const Coordinate &ca, const Vertex &va, const double da,
                                   const Coordinate &cb, const Vertex &vb, const double db)
            {
                assert(ds*da<0);
                assert(ds*db<0);
                const Point head = couple(cs,vs,ds,ca,va,da);
                const Point tail = couple(cs,vs,ds,cb,vb,db);
                slist.push_back( new Segment(head,tail) );
            }


            void Level_:: check() const
            {
                const unsigned indx = unsigned(index);
                unsigned       is1  = 1;
                for(const Segment *s = slist.head; s; s=s->next, ++is1 )
                {
                    if(! search(s->head->key()) ) throw exception("%slevel#%u: invalid segment#%u.head",Fn,indx,is1);
                    if(! search(s->tail->key()) ) throw exception("%slevel#%u: invalid segment#%u.tail",Fn,indx,is1);
                    unsigned       is2 = is1;
                    const Segment *nxt = s->next;
                    while(nxt)
                    {
                        ++is2;
                        if( *s == *nxt )
                        {
                            throw exception("%slevel%u: same segment#%u and segment#%u",Fn,indx,is1,is2);
                        }
                        nxt=nxt->next;
                    }
                }
            }

        }
    }
}
