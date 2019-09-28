#include "y/geometry/contour2d.hpp"
#include "y/exception.hpp"
#include "y/type/bzset.hpp"

namespace upsylon {

    namespace geometry {

        static const char Fn[] = "contour2d::level: ";

        contour2d:: level_:: level_( const size_t k ) throw() :
        index(k),
        slist(),
        iso()
        {

        }

        contour2d:: level_:: ~level_() throw()
        {
            bzset_(index);
        }

        const size_t & contour2d:: level_:: key() const throw()
        {
            return index;
        }

        void contour2d:: level_:: build_isolines(const bool keep)
        {
            slist.build(iso,keep);
        }

        contour2d::point_  *  contour2d:: level_:: single( const coordinate &c, const vertex &v )
        {
            const edge   e(c);
            point       *pp = search(e);
            if(pp)
            {

                return &(**pp);
            }
            else
            {
                point_ *p = new point_(e,v);
                {
                    const point tmp = p;
                    if(!insert(tmp)) throw exception("%scan't insert new point!!!",Fn);
                }
                return p;
            }
        }



        contour2d::point_ *contour2d::level_:: couple(const coordinate &ca, const vertex &va, const double da,
                                                      const coordinate &cb, const vertex &vb, const double db)
        {
            assert(ca!=cb);
            assert(da*db<0);
            const edge e(ca,cb);
            point *pp = search(e);
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
                const vertex vz  = va+lam*(vb-va);
                point_ *p = new point_(e,vz);
                {
                    const point tmp = p;
                    if(!insert(tmp)) throw exception("%scan't insert new point!!!",Fn);
                }
                return p;
            }
        }

        void     contour2d:: level_:: full(const coordinate &ca, const vertex &va,
                                           const coordinate &cb, const vertex &vb)
        {
            const point head = single(ca,va);
            const point tail = single(cb,vb);
            slist.push_back( new segment(head,tail) );
        }

        void    contour2d:: level_:: inter1(const coordinate &cz, const vertex &vz,
                                            const coordinate &ca, const vertex &va, const double da,
                                            const coordinate &cb, const vertex &vb, const double db)
        {
            const point head = single(cz,vz);
            const point tail = couple(ca,va,da,cb,vb,db);
            slist.push_back( new segment(head,tail) );
        }

        void   contour2d:: level_:: inter2(const coordinate &cs, const vertex &vs, const double ds,
                                           const coordinate &ca, const vertex &va, const double da,
                                           const coordinate &cb, const vertex &vb, const double db)
        {
            assert(ds*da<0);
            assert(ds*db<0);
            const point head = couple(cs,vs,ds,ca,va,da);
            const point tail = couple(cs,vs,ds,cb,vb,db);
            slist.push_back( new segment(head,tail) );
        }


        void contour2d:: level_:: check() const
        {
            const unsigned indx = unsigned(index);
            unsigned       is1  = 1;
            for(const segment *s = slist.head; s; s=s->next, ++is1 )
            {
                if(! search(s->head->key()) ) throw exception("%slevel#%u: invalid segment#%u.head",Fn,indx,is1);
                if(! search(s->tail->key()) ) throw exception("%slevel#%u: invalid segment#%u.tail",Fn,indx,is1);
                unsigned       is2 = is1;
                const segment *nxt = s->next;
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
