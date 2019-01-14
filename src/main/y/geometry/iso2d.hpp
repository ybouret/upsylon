#ifndef Y_GEOMETRY_ISO2D_INCLUDED
#define Y_GEOMETRY_ISO2D_INCLUDED 1


#include "y/type/point2d.hpp"
#include "y/associative/set.hpp"
#include "y/ptr/arc.hpp"
#include "y/ptr/intr.hpp"
#include "y/sequence/vector.hpp"

#include "y/exception.hpp"

namespace upsylon
{
    namespace geometry
    {

        //! findind iso levels in 2d
        /**
         Each square is divided in four triangles
         4@(i  ,j+1)------3@(i+1,j+1)
         | \           /  |
         |   \       /    |
         |     \   /      |
         |       0        |
         |     /   \      |
         |   /       \    |
         | /           \  |
         1@(i ,j   )-----2@(i+1,j  )

         For a given level, a matching value can be found on a vertex or
         on within an edge, which defines a set of unique coordinates
         for any point of the isoline
         */
        struct iso2d
        {
            typedef point2d<double> vertex;

            //__________________________________________________________________
            //
            // iso level is on a vertex
            //__________________________________________________________________
            static const unsigned vertex0 = 0x01<<0; //!< bit 1
            static const unsigned vertex1 = 0x01<<1; //!< bit 2
            static const unsigned vertex2 = 0x01<<2; //!< bit 3
            static const unsigned vertex3 = 0x01<<3; //!< bit 4
            static const unsigned vertex4 = 0x01<<4; //!< bit 5
            
            //! identifier for a point of an iso level
            class identifier
            {
            public:
                const unit_t   i; //!< index of the square
                const unit_t   j; //!< index of the square
                const unsigned p; //!< position within the square

                identifier(const unit_t ii, const unit_t jj, const unsigned pp) throw();
                identifier(const identifier &other) throw();
                ~identifier() throw();

                friend bool operator==(const identifier &lhs, const identifier &rhs) throw()
                {
                    return (lhs.i==rhs.i) && (lhs.j==rhs.j) && (lhs.p==rhs.p);
                }

                inline friend std::ostream & operator<<( std::ostream &os, const identifier &id )
                {
                    os << "[" << id.i << "," << id.j << "@" << id.p << "]";
                    return os;
                }

            private:
                Y_DISABLE_ASSIGN(identifier);
            public:
                class hasher
                {
                public:
                    hashing::fnv H;
                    hasher() throw();
                    ~hasher() throw();
                    size_t operator()(const identifier &id) throw();

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(hasher);
                };
            };

            //! a unique point is an identifier and its position
            class unique_point : public counted_object
            {
            public:
                const identifier tag; //!< logical  identifier
                const vertex     vtx; //!< physical position

                explicit unique_point(const identifier &id, const vertex &v) throw();
                virtual ~unique_point() throw();
                const identifier & key() const throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(unique_point);
            };

            typedef intr_ptr<identifier,unique_point>               shared_point;
            typedef set<identifier,shared_point,identifier::hasher> unique_points;

            class segment : public object
            {
            public:
                typedef core::list_of_cpp<segment> list;
                segment           *next;
                segment           *prev;
                const shared_point a;
                const shared_point b;
                explicit segment(unique_point       *pa, unique_point     *pb) throw() :
                next(0), prev(0), a(pa), b(pb)
                {
                    assert(a->refcount()>1); assert( object::is_block(pa) );
                    assert(b->refcount()>1); assert( object::is_block(pb) );
                }

                virtual ~segment() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(segment);
            };


            //! return the position where the field vanished, va*vb<0
            static vertex   zfind(const vertex &pa, const double va,
                                  const vertex &pb, const double vb) throw();

            class shared_points : public unique_points, public counted
            {
            public:
                explicit shared_points() throw();
                virtual ~shared_points() throw();

                segment::list segments;
                unit_t        i,j;

                //! get one point or create it from fallback
                unique_point * operator()(const unsigned p, const vertex &fallback)
                {
                    const identifier tag(i,j,p);
                    shared_point    *psp = search(tag);
                    if( 0!=psp )
                    {
                        return & **psp;
                    }
                    else
                    {
                        unique_point      *up = new unique_point(tag,fallback);
                        const shared_point sp = up; assert(tag==sp->tag);
                        if(!insert(sp)) throw exception("unexpected multiple iso2d::vertex @(%d,%d,+%u)!",int(i), int(j), p);
                        return up;
                    }
                }

                //! get one point or create it from two points
                unique_point * operator()(const unsigned q0, const vertex &p0, const double v0,
                                          const unsigned q1, const vertex &p1, const double v1)
                {
                    const identifier tag(i,j,q0|q1);
                    shared_point *psp = search(tag);
                    if( psp )
                    {
                        assert(tag== (**psp).tag);
                        shared_point &sp = *psp;
                        unique_point &up = *sp;
                        return &up;
                    }
                    else
                    {
                        const vertex       vv = zfind(p0, v0, p1, v1);
                        unique_point      *up = new unique_point(tag,vv);
                        const shared_point sp = up;
                        if(!insert(sp)) throw exception("unexpected multiple iso2d::vertex @(%d,%d,+%u,+%u)!", int(i), int(j), q0,q1);
                        return up;
                    }
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(shared_points);
            };

            typedef arc_ptr<shared_points> level;
            typedef vector<level>          levels;



            static const unsigned       sign_bits     = 2;           //!< bits to encode sign
            static const unsigned       sign_shift0   = 0*sign_bits; //!< shift for vertex 0
            static const unsigned       sign_shift1   = 1*sign_bits; //!< shift for vertex 1
            static const unsigned       sign_shift2   = 2*sign_bits; //!< shift for vertex 2
            static const unsigned       sign_zero     = 0x00;        //!< encoding 0
            static const unsigned       sign_positive = 0x01;        //!< encoding >0
            static const unsigned       sign_negative = 0x02;        //!< encoding <0

            static const unsigned zzz0 = sign_zero     << sign_shift0; //!< f0=0
            static const unsigned pos0 = sign_positive << sign_shift0; //!< f0>0
            static const unsigned neg0 = sign_negative << sign_shift0; //!< f0<0

            static const unsigned zzz1 = sign_zero     << sign_shift1; //!< f1=0
            static const unsigned pos1 = sign_positive << sign_shift1; //!< f1>0
            static const unsigned neg1 = sign_negative << sign_shift1; //!< f1<0

            static const unsigned zzz2 = sign_zero     << sign_shift2; //!< f2=0
            static const unsigned pos2 = sign_positive << sign_shift2; //!< f2>0
            static const unsigned neg2 = sign_negative << sign_shift2; //!< f2<0

            //! return an unshifted flag
            static unsigned sign_flag( const double value ) throw();



            //! register a new segment

            //! a low-level contour algorithm
            /**
             data            !  matrix/field of data to contour
             ilb,iub,jlb,jub ! index bounds of data matrix[j][i]
             x               ! data matrix column coordinates
             y               ! data matrix row coordinates
             z               ! contour levels in INCREASING order, nc=z.size()
             */
            template<
            typename FIELD,
            typename ARRAY
            > static inline
            void scan(levels              &lvl,
                      const FIELD         &data,
                      const unit_t         ilb,
                      const unit_t         iub,
                      const unit_t         jlb,
                      const unit_t         jub,
                      const ARRAY         &x,
                      const ARRAY         &y,
                      const array<double> &z
                      )
            {
                static const size_t tri[4][2] =
                {
                    {1,2},
                    {2,3},
                    {3,4},
                    {4,1}
                };

                const size_t nc = z.size();
#if !defined(NDEBUG)
                for(size_t i=1;i<nc;++i)
                {
                    assert(z[i]<=z[i+1]);
                }
#endif
                if(nc<=0)
                {
                    lvl.free();
                    return;
                }

                //--------------------------------------------------------------
                //
                // prepare levels
                //
                //--------------------------------------------------------------
                lvl.ensure(nc);
                while(lvl.size()>nc)
                {
                    lvl.pop_back();
                }
                while(lvl.size()<nc)
                {
                    const level _ = new shared_points();
                    lvl.push_back( _ );
                }
                assert(nc==lvl.size());
                for(size_t k=nc;k>0;--k)
                {
                    lvl[k]->free();
                }
                const double zlo = z[1];
                const double zhi = z[nc];

                //--------------------------------------------------------------
                //
                // loop over mesh
                //
                //--------------------------------------------------------------
                for(unit_t j0=jlb,j1=jlb+1;j0<jub;++j0,++j1)
                {
                    const double y0=y[j0];
                    const double y1=y[j1];
                    const double ymid = 0.5*(y0+y1);
                    for(unit_t i0=ilb,i1=i0+1;i0<iub;++i0,++i1)
                    {
                        const double x0 = x[i0];
                        const double x1 = x[i1];

                        //------------------------------------------------------
                        // load global data
                        //------------------------------------------------------
                        double g[5] = { 0,0,0,0,0 };
                        double dmin = (g[1]=double(data[j0][i0])),      dmax=dmin;
                        dmin = min_of( g[2]=double(data[j0][i1]),dmin); dmax=max_of(dmax,g[2]);
                        dmin = min_of( g[3]=double(data[j1][i1]),dmin); dmax=max_of(dmax,g[3]);
                        dmin = min_of( g[4]=double(data[j1][i0]),dmin); dmax=max_of(dmax,g[4]);
                        if(dmax<zlo||dmin>zhi)
                        {
                            continue;
                        }

                        //------------------------------------------------------
                        // global coordinates
                        //------------------------------------------------------
                        const vertex vtx[5] =
                        {
                            vertex(0.5*(x0+x1),ymid),
                            vertex(x0,y0),
                            vertex(x1,y0),
                            vertex(x1,y1),
                            vertex(x0,y1)
                        };

                        //------------------------------------------------------
                        // loop over levels
                        //------------------------------------------------------
                        for(size_t k=nc;k>0;--k)
                        {
                            const double  zk   = z[k];
                            const double  f[5] = { 0, g[1]-zk, g[2]-zk, g[3]-zk, g[4]-zk };
                            shared_points &db       = *lvl[k];
							segment::list &segments = db.segments;
                            db.i = i0;
                            db.j = j0;

                            //--------------------------------------------------
                            // loop over triangles
                            //--------------------------------------------------
                            static const size_t m0   = 0;
                            const vertex        p0   = vtx[m0];
                            const double        f0   = 0.25*(f[1]+f[2]+f[3]+f[4]);
                            const unsigned      q0   = 0x01<<m0;
                            const unsigned      s0   = (sign_flag(f0) << sign_shift0);
                            for(size_t l=0;l<4;++l)
                            {
                                //----------------------------------------------
                                // get indices
                                //----------------------------------------------
                                const size_t        m1 = tri[l][0];
                                const size_t        m2 = tri[l][1];
                                const unsigned      q1 = 0x01 << m1;
                                const unsigned      q2 = 0x01 << m2;
                                
                                //----------------------------------------------
                                // get real coordinates
                                //----------------------------------------------
                                const vertex    p1    = vtx[m1];
                                const vertex    p2    = vtx[m2];

                                const double   f1    = f[m1];
                                const double   f2    = f[m2];
                                const unsigned flags = s0 | (sign_flag(f1)<<sign_shift1) | (sign_flag(f2)<<sign_shift2);

                                //----------------------------------------------
                                //
                                // there are nine symetrical possibilities
                                //
                                //----------------------------------------------
                                switch(flags)
                                {
                                        //--------------------------------------
                                        //
                                        // intercepting two edges
                                        //
                                        //--------------------------------------
                                        // (p0/p1) - (p0/p2)
                                    case neg0|pos1|pos2:
                                    case pos0|neg1|neg2:
                                        segments.push_back( new segment(db(q0,p0,f0,q1,p1,f1),db(q0,p0,f0,q2,p2,f2)) );
                                        break;

                                        // (p0/p1) - (p1/p2)
                                    case neg0|pos1|neg2:
                                    case pos0|neg1|pos2:
                                        segments.push_back( new segment(db(q0,p0,f0,q1,p1,f1),db(q1,p1,f1,q2,p2,f2)) );
                                        //cb( zfind(p0,f0,p1,f1), zfind(p1,f1,p2,f2) );
                                        break;

                                        // (p0/p2) - (p1/p2)
                                    case neg0|neg1|pos2:
                                    case pos0|pos1|neg2:
                                        segments.push_back( new segment(db(q0,p0,f0,q2,p2,f2),db(q1,p1,f1,q2,p2,f2)) );
                                        break;

                                        //--------------------------------------
                                        //
                                        // vertex+intercep on one edge
                                        //
                                        //--------------------------------------

                                        // p0 - (p1/p2)
                                    case zzz0|pos1|neg2:
                                    case zzz0|neg1|pos2:
                                        segments.push_back( new segment(db(q0,p0),db(q1,p1,f1,q2,p2,f2)) );
                                        break;

                                        // p1 - (p0/p2)
                                    case pos0|zzz1|neg2:
                                    case neg0|zzz1|pos2:
                                        segments.push_back( new segment(db(q1,p1),db(q0,p0,f0,q2,p2,f2)) );
                                        break;

                                        // p2 - (p0/p1)
                                    case pos0|neg1|zzz2:
                                    case neg0|pos1|zzz2:
                                        segments.push_back( new segment(db(q2,p2),db(q0,p0,f0,q1,p1,f1)) );
                                        break;

                                        //--------------------------------------
                                        //
                                        // one of the full edge
                                        //
                                        //--------------------------------------

                                        // p0-p1
                                    case zzz0|zzz1|pos2:
                                    case zzz0|zzz1|neg2:
                                        db.segments.push_back( new segment(db(q0,p0),db(q0,p0)) );
                                        break;

                                        // p1-p2
                                    case neg0|zzz1|zzz2:
                                    case pos0|zzz1|zzz2:
                                        db.segments.push_back( new segment(db(q1,p1),db(q2,p2)) );
                                        break;

                                        // p0-p2
                                    case zzz0|neg1|zzz2:
                                    case zzz0|pos1|zzz2:
                                        db.segments.push_back( new segment(db(q0,p0),db(q2,p2)) );
                                        break;

                                    default:
                                        // no creating...
                                        break;
                                }

                            }

                        }

                    }
                    
                }


            }
            

        };

    }
}

#endif

