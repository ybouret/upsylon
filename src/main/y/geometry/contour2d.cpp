
#include "y/geometry/contour2d.hpp"
#include "y/type/bzset.hpp"
#include "y/comparison.hpp"
#include "y/type/bswap.hpp"
#include "y/sort/sorted-sum.hpp"

namespace  {
    static const char Fn[] = "contour2d: ";
}



namespace upsylon {
    
    namespace geometry {
        
        const contour2d::edge & contour2d:: point_:: key() const throw()
        {
            return location;
        }
        
        contour2d::point_:: ~point_() throw()
        {
        }
        
        contour2d::point_:: point_(const edge &l, const vertex &p) throw() :
        location(l),
        position(p)
        {
        }
    }
    
}





#include "y/exception.hpp"
namespace upsylon {
    
    namespace geometry {
        
        contour2d:: level_set:: level_set() throw()
        {
        }
        
        contour2d:: level_set:: ~level_set() throw()
        {
        }
        
        void contour2d:: level_set:: create(const size_t n)
        {
            std::cerr << "create level_set" << std::endl;
            free();
            ensure(n);
            for(size_t k=1;k<=n;++k)
            {
                const level l = new level_(k);
                if( !insert(l) )
                {
                    throw exception("%sinsert#%u failure", Fn,unsigned(k) );
                }
            }
            assert(size()==n);
        }
        
        void contour2d:: level_set::  check() const
        {
            for( const_iterator it=begin();it!=end();++it)
            {
                (**it).check();
            }
        }
        
        void contour2d:: level_set:: build_isolines(const bool keep)
        {
            for( iterator it=begin();it!=end();++it)
            {
                (**it).build_isolines(keep);
            }
        }

        
        
    }
}

namespace upsylon {
    
    namespace geometry {
        
        namespace {
            static const unsigned n0 = 0x0001; // 1
            static const unsigned z0 = 0x0002; // 2
            static const unsigned p0 = 0x0004; // 4
            
            static const unsigned n1 = 0x0008; // 8
            static const unsigned z1 = 0x0010; // 16
            static const unsigned p1 = 0x0020; // 32
            
            static const unsigned n2 = 0x0040; // 64
            static const unsigned z2 = 0x0080; // 128
            static const unsigned p2 = 0x0100; // 256
        }
        
        
#define Y_CONTOUR2D_FLAG(I) do { \
switch(contour::sign_of(d##I))\
{\
case contour::is_negative:  flags |= n##I; break;\
case contour::is_zero:      flags |= z##I; break;\
case contour::is_positive:  flags |= p##I; break;\
} } while(false)
        
        void contour2d:: scan_triangles(context &ctx)
        {
            static const unsigned i0       = 0;
            static const unsigned ii[4][2] = { {1,2}, {2,3}, {3,4}, {4,1} };
            
            //------------------------------------------------------------------
            //
            // Loop over the four triangles
            //
            //------------------------------------------------------------------
            assert(ctx.c);
            assert(ctx.d);
            assert(ctx.v);
            assert(ctx.l);
            level_ &L = *ctx.l;
            for(size_t t=0;t<4;++t)
            {
                const unsigned i1 = ii[t][0];
                const unsigned i2 = ii[t][1];
                
                const double   d0 = ctx.d[i0];
                const double   d1 = ctx.d[i1];
                const double   d2 = ctx.d[i2];
                
                
                
                const coordinate &c0 = ctx.c[i0];
                const coordinate &c1 = ctx.c[i1];
                const coordinate &c2 = ctx.c[i2];
                
                const vertex     &v0 = ctx.v[i0];
                const vertex     &v1 = ctx.v[i1];
                const vertex     &v2 = ctx.v[i2];
                
                
                unsigned flags = 0;
                Y_CONTOUR2D_FLAG(0);
                Y_CONTOUR2D_FLAG(1);
                Y_CONTOUR2D_FLAG(2);
                
                switch(flags)
                {
                        //------------------------------------------------------
                        //
                        // almost do nothing cases 0+3=3/27
                        //
                        //------------------------------------------------------
                    case z0|z1|z2:  (void) L.single(c0,v0);  (void) L.single(c1,v1);  (void) L.single(c2,v2);
                    case n0|n1|n2:
                    case p0|p1|p2:
                        break;
                        
                        //------------------------------------------------------
                        //
                        // create single point, no segment cases
                        // 3+6=9/27
                        //
                        //------------------------------------------------------
                    case n0|n1|z2:
                    case p0|p1|z2:
                        (void) L.single(c2,v2);
                        break;
                        
                    case n0|z1|n2:
                    case p0|z1|p2:
                        (void) L.single(c1,v1);
                        break;
                        
                    case z0|n1|n2:
                    case z0|p1|p2:
                        (void) L.single(c0,v0);
                        break;
                        
                        
                        //------------------------------------------------------
                        //
                        // create segment=full edge
                        // 9+6=15/27
                        //
                        //------------------------------------------------------
                    case z0|z1|n2:
                    case z0|z1|p2:
                        L.full(c0,v0,c1,v1);
                        break;
                        
                    case z0|n1|z2:
                    case z0|p1|z2:
                        L.full(c0,v0,c2,v2);
                        break;
                        
                    case n0|z1|z2:
                    case p0|z1|z2:
                        L.full(c1,v1,c2,v2);
                        break;
                        
                        
                        //------------------------------------------------------
                        //
                        // create segment=point+intersection
                        // 15+6=21/27
                        //
                        //------------------------------------------------------
                    case z0|p1|n2:
                    case z0|n1|p2:
                        L.inter1(c0,v0, c1,v1,d1, c2,v2,d2);
                        break;
                        
                    case n0|z1|p2:
                    case p0|z1|n2:
                        L.inter1(c1,v1, c0,v0,d0, c2,v2,d2);
                        break;
                        
                    case n0|p1|z2:
                    case p0|n1|z2:
                        L.inter1(c2,v2, c0,v0,d0, c1,v1,d1);
                        break;
                        
                        //------------------------------------------------------
                        //
                        // create segment=intersection+intersection
                        // 21+6=27/27
                        //
                        //------------------------------------------------------
                    case n0|p1|p2:
                    case p0|n1|n2:
                        L.inter2(c0,v0,d0, c1,v1,d1, c2,v2,d2);
                        break;
                        
                    case n0|p1|n2:
                    case p0|n1|p2:
                        L.inter2(c1,v1,d1, c0,v0,d0, c2,v2,d2);
                        break;
                        
                    case n0|n1|p2:
                    case p0|p1|n2:
                        L.inter2(c2,v2,d2, c0,v0,d0, c1,v1,d1);
                        break;
                        
                        
                    default:
                        throw exception("%sscan(unexpected case)",Fn);
                        //break;
                        
                }
                
            }
        }
        
        
    }
    
}

namespace upsylon {
    
    namespace geometry {
        
        contour2d:: isopoint:: isopoint( const point &p ) throw() :
        point(p), next(0), prev(0)
        {
        }
        
        contour2d:: isopoint:: ~isopoint() throw()
        {
        }
        
    }
}

namespace upsylon {
    
    namespace geometry {
        
        contour2d:: isoline_:: isoline_() throw() :
        cyclic(false)
        {
        }
        
        contour2d:: isoline_:: ~isoline_() throw()
        {
        }
        
        
        
    }
}


namespace upsylon {
    
    namespace geometry {

        bool contour2d:: segments::  grow( isolines &iso, const segment *s )
        {
            assert(s);
            const point &s_head = s->head;
            const point &s_tail = s->tail;
            for(size_t i=iso.size();i>0;--i)
            {
                isoline_    &C      = *iso[i]; assert(C.size>=2);
                const point &c_head = *(C.head);
                const point &c_tail = *(C.tail);

                if(c_head==s_head)
                {
                    C.push_front( new isopoint(s_tail) );
                    return true;
                }

                if(c_head==s_tail)
                {
                    C.push_front( new isopoint(s_head) );
                    return true;
                }

                if(c_tail==s_head)
                {
                    C.push_back( new isopoint(s_tail) );
                    return true;
                }

                if(c_tail==s_tail)
                {
                    C.push_back( new isopoint(s_head) );
                    return true;
                }
            }
            return false;
        }


        void contour2d:: segments:: build( isolines &iso, const bool keep )
        {
            iso.free();
            goto TRY_GROW;

        KEEP:
            assert(0==size);
            if(keep)
            {
                for(size_t i=1;i<=iso.size();++i)
                {
                    const isopoint *p = iso[i]->head;
                    while(p&&p->next)
                    {
                        push_back( new segment( *p, *(p->next) ) );
                        p=p->next;
                    }
                }

            }

            for(size_t i=1;i<=iso.size();++i)
            {
                const isoline_ &l = *iso[i];
                const isopoint &p = *(l.head);
                const isopoint &q = *(l.tail);
                (bool &)l.cyclic = (p==q);
            }

            return;

        TRY_GROW:
            {
                //--------------------------------------------------------------
                //
                // try to place all remaining segments
                //
                //--------------------------------------------------------------
                segments store;         // local store
                bool     grown = false; // check if something was grown...
                while(size>0)
                {
                    if(grow(iso,head))
                    {
                        grown = true;
                        delete pop_front();
                    }
                    else
                    {
                        store.push_back( pop_front() );
                    }
                }

                //--------------------------------------------------------------
                //
                // check status
                //
                //--------------------------------------------------------------

                if(store.size<=0)
                {
                    //__________________________________________________________
                    //
                    // early return, nothing left
                    //__________________________________________________________
                    goto KEEP;
                }
                else
                {
                    //__________________________________________________________
                    //
                    // still some segments to place
                    //__________________________________________________________
                    if(grown)
                    {
                        //______________________________________________________
                        //
                        // iso has changed, try again
                        //______________________________________________________
                        assert(store.size>0);
                        swap_with(store);
                        goto TRY_GROW;
                    }
                    else
                    {
                        //______________________________________________________
                        //
                        // nothing has changed, need a new curve
                        //______________________________________________________
                        assert(store.size>0);
                        isoline        I = new isoline_();
                        const segment *s = store.head;
                        I->push_back( new isopoint(s->head) );
                        I->push_back( new isopoint(s->tail) );
                        iso.push_back(I);
                        delete store.pop_front();
                        if(store.size<=0)
                        {
                            goto KEEP;      // second early return
                        }
                        else
                        {
                            swap_with(store);
                            goto TRY_GROW; // try with new topology
                        }
                    }

                }

            }



        }
        
    }
}

