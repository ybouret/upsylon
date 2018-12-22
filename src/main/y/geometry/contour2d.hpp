//! \file
#ifndef Y_GEOMETRY_CONTOUR2D_INCLUDED
#define Y_GEOMETRY_CONTOUR2D_INCLUDED 1

#include "y/type/point2d.hpp"
#include "y/type/ints.hpp"
#include "y/associative/set.hpp"
#include "y/ptr/intr.hpp"
#include "y/ptr/auto.hpp"
#include "y/exception.hpp"
#include "y/core/node.hpp"

#include <iostream>

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




            //! unique point identifier
            class   identifier
            {
            public:
                typedef hashing::fnv hash_function; //!< internal hashing function

                const coordinate coord; //!< coordinate on global grid
                const offset     delta; //!< offset on local grid

                explicit identifier( const coordinate &c, const offset &d ) throw(); //!< setup
                virtual ~identifier() throw();                                       //!< cleanup
                identifier(const identifier &other) throw();                         //!< copy fields

                //! test equality
                friend bool operator==( const identifier &lhs, const identifier &rhs) throw();

                //! hasher class for set
                class hasher
                {
                public:
                    hash_function h; //!< used to hash identifier fields

                    hasher() throw();  //!< setup
                    ~hasher() throw(); //!< cleanup
                    //! get the key from identifier
                    size_t operator()( const identifier &id ) throw();

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(hasher);
                };

            private:
                Y_DISABLE_ASSIGN(identifier);
            };



            //! a unique point based on its identifier
            class unique_point : public counted_object
            {
            public:
                const identifier uid; //!< unique ID
                const point      pos; //!< unique position

                //! setup
                explicit unique_point( const identifier &i, const point &p ) throw() ;
                //! cleanup
                virtual ~unique_point() throw();

                //! identifier as key
                const identifier & key() const throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(unique_point);
            };

            typedef intr_ptr<identifier,unique_point>                shared_point;      //!< alias
            typedef set<identifier,shared_point,identifier::hasher>  shared_point_db;   //!< alias

            //! a segment between two unique points
            class segment : public object
            {
            public:
                typedef core::list_of_cpp<segment> list_type; //!< alias

                segment           *next; //!< for list
                segment           *prev; //!< for list
                const shared_point a;    //!< first point
                const shared_point b;    //!< second point

                //! setup
                explicit segment( const shared_point &A, const shared_point &B ) throw();
                virtual ~segment() throw(); //!< cleanup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(segment);
            };


            typedef core::node_of<const shared_point>    shared_point_node; //!< dynamic node of shared_point
            typedef core::list_of_cpp<shared_point_node> shared_point_list; //!< alias

            class iso_line : public shared_point_list, public object
            {
            public:
                iso_line *next;
                iso_line *prev;
                explicit iso_line() throw() : next(0), prev(0) {}
                virtual ~iso_line() throw() {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(iso_line);
            };

            typedef core::list_of_cpp<iso_line> ios_lines;

            //! segments are a list of indivual segment belonging to the same level
            class segments : public segment::list_type, public counted_object
            {
            public:
                const size_t indx; //!< index in iso levels

                explicit segments( const size_t id ) throw(); //!< setup
                virtual ~segments() throw();                  //!< cleanup

                const size_t & key() const throw(); //!< index as key

                void build_connected( ios_lines &iso ) const;
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(segments);
            };

            typedef intr_ptr<size_t,segments>   shared_segments;     //!< alias
            typedef set<size_t,shared_segments> shared_segments_set; //!< alias

            //! database of shared segements
            class shared_segments_db : public shared_segments_set
            {
            public:
                //! setup
                explicit shared_segments_db(const size_t n=0) : shared_segments_set(n,as_capacity) {}
                //! cleanup
                virtual ~shared_segments_db() throw() {}

                //! create a segment and add it to a checked set of segments
                void make( const size_t indx, unique_point *pA, unique_point *pB )
                {
                    const shared_point A(pA);
                    const shared_point B(pB);
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


            //! utility class to compute identifiers
            class unique_point_dispatcher
            {
            public:
                shared_point_db  &pdb;       //!< points database
                const coordinate  coord;     //!< shared lower indices
                const point       lower;     //!< real lower values
                const point       upper;     //!< real upper values

                //! setup
                explicit unique_point_dispatcher(shared_point_db  &_pdb,
                                                 const coordinate &_coord,
                                                 const point      &_lower,
                                                 const point      &_upper) :
                pdb( _pdb ), coord( _coord ), lower( _lower ), upper( _upper )
                {

                }

                //! cleanup
                ~unique_point_dispatcher() throw()
                {
                }

                //! compute offset based on resolution
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

                //! check/create the point into database
                unique_point *operator()( const point p )
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

                //! check/create the interpolated point into database
                unique_point * operator()(const point &pa,
                                          const double va,
                                          const point &pb,
                                          const double vb )
                {
                    assert(va*vb<=0);
                    const double  den = vb-va;
                    if(fabs(den)<=0)
                    {
                        const point q( 0.5*(pa.x+pb.x), 0.5*(pa.y+pb.y) );
                        return (*this)(q);
                    }
                    else
                    {
                        const double  lam = -va / (vb-va);
                        const point   q( pa.x + lam*(pb.x-pa.x), pa.y+lam*(pb.y-pa.y) );
                        return (*this)(q);
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
                        if(dmax<zlo||dmin>zhi)
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
                            f[0] = 0.25*(f[1]+f[2]+f[3]+f[4]);
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
                                //std::cerr << sign_flag(f0) << "|" << sign_flag(f1) << "|" << sign_flag(f2) << std::endl;

                                //----------------------------------------------
                                //
                                // there are nine symetrical possibilities
                                //----------------------------------------------
                                switch(flags)
                                {
                                        // intercepting
                                    case neg0|pos1|pos2:
                                    case pos0|neg1|neg2:
                                        sdb.make(k, mgr(p0,f0,p1,f1), mgr(p0,f0,p2,f2));
                                        break;

                                    case neg0|pos1|neg2:
                                    case pos0|neg1|pos2:
                                        sdb.make(k, mgr(p0,f0,p1,f1), mgr(p1,f1,p2,f2) );
                                        break;

                                    case neg0|neg1|pos2:
                                    case pos0|pos1|neg2:
                                        sdb.make(k, mgr(p0,f0,p2,f2), mgr(p1,f1,p2,f2) );
                                        break;

                                        // vertex+intercep
                                    case zzz0|pos1|neg2:
                                    case zzz0|neg1|pos2:
                                        sdb.make(k,mgr(p0),mgr(p1,f1,p2,f2));
                                        break;

                                    case pos0|zzz1|neg2:
                                    case neg0|zzz1|pos2:
                                        sdb.make(k,mgr(p1),mgr(p0,f0,p2,f2));
                                        break;

                                    case pos0|neg1|zzz2:
                                    case neg0|pos1|zzz2:
                                        sdb.make(k,mgr(p2), mgr(p1,f1,p2,f2));
                                        break;

                                        // one of the vertices
                                    case zzz0|zzz1|pos2:
                                    case zzz0|zzz1|neg2:
                                        sdb.make(k,mgr(p0),mgr(p1));
                                        break;

                                    case neg0|zzz1|zzz2:
                                    case pos0|zzz1|zzz2:
                                        sdb.make(k,mgr(p1),mgr(p2));
                                        break;

                                    case zzz0|neg1|zzz2:
                                    case zzz0|pos1|zzz2:
                                        sdb.make(k,mgr(p0),mgr(p2));
                                        break;

                                    default: break;
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

