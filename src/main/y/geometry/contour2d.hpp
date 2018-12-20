//! \file
#ifndef Y_GEOMETRY_CONTOUR2D_INCLUDED
#define Y_GEOMETRY_CONTOUR2D_INCLUDED 1

#include "y/type/point2d.hpp"
#include "y/type/ints.hpp"
#include "y/associative/set.hpp"
#include "y/ptr/intr.hpp"
#include "y/ptr/auto.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace geometry
    {

        //! data structure for 2d contour scanning
        struct contour2d
        {

            typedef point2d<double>     point;      //!< point physical coordinate
            typedef uint32_t            resolution; //!< point resolution type insite it's square
            static  const resolution    resolution_max = limit_of<resolution>::maximum; //!< top max [0..1] with [0..resolution_max]
            typedef point2d<unit_t>     coordinate; //!< (lower)coordinate of a square
            typedef point2d<resolution> offset;     //!< point.x = x[coordinate.x] + (x[coordinate.x+1] - x[coordinate.x]) * offset.x / resolution_max

            static const unsigned       sign_bits     = 2;
            static const unsigned       sign_shift0   = 0*sign_bits;
            static const unsigned       sign_shift1   = 1*sign_bits;
            static const unsigned       sign_shift2   = 2*sign_bits;
            static const unsigned       sign_zero     = 0x00;
            static const unsigned       sign_positive = 0x01;
            static const unsigned       sign_negative = 0x02;

            static const unsigned zzz0 = sign_zero     << sign_shift0;
            static const unsigned pos0 = sign_positive << sign_shift0;
            static const unsigned neg0 = sign_negative << sign_shift0;

            static const unsigned zzz1 = sign_zero     << sign_shift1;
            static const unsigned pos1 = sign_positive << sign_shift1;
            static const unsigned neg1 = sign_negative << sign_shift1;

            static const unsigned zzz2 = sign_zero     << sign_shift2;
            static const unsigned pos2 = sign_positive << sign_shift2;
            static const unsigned neg2 = sign_negative << sign_shift2;

            static inline
            unsigned sign_flag( const double value ) throw()
            {
                if(value<0)      return sign_negative;
                else if(value>0) return sign_positive;
                else             return sign_zero;
            }
            //! unique point identifier
            class   identifier
            {
            public:
                const coordinate coord;
                const offset     delta;
                inline  identifier( const coordinate &c, const offset &d ) throw() : coord(c), delta(d) {}
                inline ~identifier() throw() {}
                inline  identifier(const identifier &other) throw() : coord(other.coord), delta(other.delta) {}
                inline  friend bool operator==( const identifier &lhs, const identifier &rhs) throw()
                {
                    return (lhs.coord==rhs.coord) && (lhs.delta==rhs.delta);
                }
                typedef hashing::fnv hash_function;

                class hasher
                {
                public:
                    inline  hasher() throw() : h() {}
                    inline ~hasher() throw() {}
                    inline size_t operator()( const identifier &id ) throw()
                    {
                        h.set();
                        h.run(&id.coord,sizeof(coordinate));
                        h.run(&id.delta,sizeof(offset));
                        return h.key<size_t>();
                    }

                    hash_function h;
                private:
                    Y_DISABLE_COPY_AND_ASSIGN(hasher);
                };

            private:
                Y_DISABLE_ASSIGN(identifier);
            };

            class unique_point : public counted_object
            {
            public:
                const identifier uid;
                const point      pos;

                inline  unique_point( const identifier &i, const point &p ) throw() : uid(i), pos(p) {}
                inline ~unique_point() throw() {}

                const identifier & key() const throw() { return uid; }
            private:
                Y_DISABLE_COPY_AND_ASSIGN(unique_point);
            };

            typedef intr_ptr<identifier,unique_point>                shared_point;
            typedef set<identifier,shared_point,identifier::hasher>  shared_point_db;

            class segment : public object
            {
            public:
                segment           *next;
                segment           *prev;
                const shared_point a;
                const shared_point b;

                inline explicit segment( const shared_point &A, const shared_point &B ) throw() : next(0), prev(0), a(A), b(B) {}
                inline virtual ~segment() throw() {}

                typedef core::list_of_cpp<segment> list_type;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(segment);
            };


            class segments : public segment::list_type, public counted_object
            {
            public:

                const size_t indx;
                explicit segments( const size_t id ) throw() : segment::list_type(), indx(id) {}
                virtual ~segments() throw() {}

                const size_t & key() const throw() { return indx; }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(segments);
            };

            typedef intr_ptr<size_t,segments>   shared_segments;
            typedef set<size_t,shared_segments> shared_segments_set;

            class shared_segments_db : public shared_segments_set
            {
            public:
                explicit shared_segments_db(const size_t n=0) : shared_segments_set(n,as_capacity) {}
                virtual ~shared_segments_db() throw() {}

                void make( const size_t indx, const shared_point &A, const shared_point &B )
                {
                    auto_ptr<segment> seg  = new segment(A,B);
                    shared_segments  *ppS  = search(indx);
                    if(ppS)
                    {
                        (**ppS).push_back(seg.yield());
                    }
                    else
                    {
                        segments *S = new segments(indx);
                        {
                            const shared_segments q = S;
                            if(!insert(q)) throw exception("failure to insert new segmensts");
                        }
                        S->push_back(seg.yield());
                    }
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(shared_segments_db);
            };


            class unique_point_dispatcher
            {
            public:
                shared_point_db  &pdb;
                const coordinate  coord;
                const point       lower;
                const point       upper;

                explicit unique_point_dispatcher(shared_point_db  &_pdb,
                                                 const coordinate &_coord,
                                                 const point      &_lower,
                                                 const point      &_upper) :
                pdb( _pdb ), coord( _coord ), lower( _lower ), upper( _upper )
                {

                }

                ~unique_point_dispatcher() throw()
                {
                }

                inline
                offset compute_offset(const point &p) const throw()
                {
                    static const double fac = resolution_max;
                    const double     ox = floor( fac*(p.x-lower.x)/(upper.x-lower.x) + 0.5 );
                    const double     oy = floor( fac*(p.y-lower.y)/(upper.y-lower.y) + 0.5 );
                    const resolution rx = resolution(ox);
                    const resolution ry = resolution(oy);
                    return offset(rx,ry);
                }

                unique_point *operator()( const point &p )
                {
                    const offset      delta = compute_offset(p);
                    const identifier  ident(coord,delta);
                    shared_point     *shpp  = pdb.search(ident);
                    if(shpp)
                    {
                        return & (**shpp);
                    }
                    else
                    {
                        unique_point      *pp  = new unique_point(ident,p);
                        const shared_point shp = pp;
                        if(!pdb.insert(shp))
                        {
                            throw exception("unexpected insert failure");
                        }
                        return pp;
                    }
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(unique_point_dispatcher);
            };


            //! a low-level contour algorithm
            /**
             data            !  matrix/field of data to contour
             ilb,iub,jlb,jub ! index bounds of data matrix[j][i]
             x               ! data matrix column coordinates
             y               ! data matrix row coordinates
             z               ! contour levels in increasing order, nc=z.size()
             */
            template<
            typename FIELD,
            typename ARRAY
            > static inline
            void scan(shared_segments_db  &sdb,
                      const FIELD         &data,
                      const unit_t         ilb,
                      const unit_t         iub,
                      const unit_t         jlb,
                      const unit_t         jub,
                      const ARRAY         &x,
                      const ARRAY         &y,
                      const array<double> &z,
                      shared_point_db     &pdb
                      )
            {
                static const size_t tri[4][2] =
                {
                    {1,2},
                    {2,3},
                    {3,4},
                    {4,1}
                };

                double g[5] = {0,0,0,0,0}; // global fields value
                double f[5] = {0,0,0,0,0}; // local fields value
                //--------------------------------------------------------------
                //
                // initialize
                //
                //--------------------------------------------------------------
                pdb.free();
                const size_t nc  = z.size();
                if(nc<=0)
                {
                    return;
                }
                const double zlo = z[1];
                const double zhi = z[nc];
#if !defined(NDEBUG)
                for(size_t i=1;i<nc;++i)
                {
                    assert(z[i]<=z[i+1]);
                }
#endif
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
                        double dmin = (g[1]=double(data[j0][i0])),      dmax=dmin;
                        dmin = min_of( g[2]=double(data[j0][i1]),dmin); dmax=max_of(dmax,g[2]);
                        dmin = min_of( g[3]=double(data[j1][i1]),dmin); dmax=max_of(dmax,g[3]);
                        dmin = min_of( g[4]=double(data[j1][i0]),dmin); dmax=max_of(dmax,g[4]);
                        if(dmin<zlo||dmax>zhi)
                        {
                            continue;
                        }

                        //------------------------------------------------------
                        // global coordinates
                        //------------------------------------------------------
                        const point lower = point(x0,y0);
                        const point upper = point(x1,y1);
                        const point vtx[5] =
                        {
                            point(0.5*(x0+x1),ymid),
                            lower,
                            point(x1,y0),
                            upper,
                            point(x0,y1)
                        };
                        const coordinate        coord(i0,j0);
                        unique_point_dispatcher mgr(pdb,coord,lower,upper);

                        //------------------------------------------------------
                        // loop over levels
                        //------------------------------------------------------
                        for(size_t k=nc;k>0;--k)
                        {
                            const double zk = z[k];
                            f[1] = g[1] - zk;
                            f[2] = g[2] - zk;
                            f[3] = g[3] - zk;
                            f[4] = g[4] - zk;
                            f[0] = 0.25*(g[1]+g[2]+g[3]+g[4]);
                            //--------------------------------------------------
                            // loop over triangles
                            //--------------------------------------------------
                            for(size_t l=0;l<4;++l)
                            {
                                //----------------------------------------------
                                // get indices
                                //----------------------------------------------
                                static const size_t m0 = 0;
                                const size_t        m1 = tri[l][0];
                                const size_t        m2 = tri[l][1];

                                //----------------------------------------------
                                // get real coordinates
                                //----------------------------------------------
                                const point    p0    = vtx[m0];
                                const point    p1    = vtx[m1];
                                const point    p2    = vtx[m2];
                                const double   f0    = f[m0];
                                const double   f1    = f[m1];
                                const double   f2    = f[m2];
                                const unsigned flags = (sign_flag(f0) << sign_shift0) | (sign_flag(f1)<<sign_shift1) | (sign_flag(f2)<<sign_shift2);
                                switch(flags)
                                {
                                    case zzz0|zzz1|zzz2: {
                                        const shared_point sp0 = mgr(p0);
                                        const shared_point sp1 = mgr(p1);
                                        const shared_point sp2 = mgr(p2);
                                        sdb.make(k,sp0,sp1);
                                        sdb.make(k,sp1,sp2);
                                        sdb.make(k,sp2,sp0);
                                    } break;

                                    default:
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

