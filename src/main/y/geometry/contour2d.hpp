//! \file
#ifndef Y_GEOMETRY_CONTOUR2D_INCLUDED
#define Y_GEOMETRY_CONTOUR2D_INCLUDED 1

#include "y/type/point2d.hpp"
#include "y/type/ints.hpp"
#include "y/associative/set.hpp"
#include "y/ptr/intr.hpp"

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
            typedef set<identifier,shared_point,identifier::hasher>  database;

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


            class segments : public segment::list_type
            {
            public:
                const size_t indx;
                explicit segments( const size_t id ) throw() : segment::list_type(), indx(id) {}
                virtual ~segments() throw() {}
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(segments);
            };



#if 0
            //! a low-level contour algorithm
            /**
             d               ! matrix/field of data to contour
             ilb,iub,jlb,jub ! index bounds of data matrix[j][i]
             x               ! data matrix column coordinates
             y               ! data matrix row coordinates
             z               ! contour levels in increasing order, nc=z.size()
             */
            template<
            typename FIELD,
            typename ARRAY
            > static inline
            void scan(const FIELD         &d,
                      const unit_t         ilb,
                      const unit_t         iub,
                      const unit_t         jlb,
                      const unit_t         jub,
                      const ARRAY         &x,
                      const ARRAY         &y,
                      const array<double> &z
                      )
            {

                static const vertex v0(res_max>>1,res_max>>1);
                static const vertex v1(0,0);
                static const vertex v2(0,res_max);
                static const vertex v3(res_max,res_max);
                static const vertex v4(res_max,0);
                double              f[5] = {0,0,0,0,0};
                double              g[5] = {0,0,0,0,0};

                // initialize
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
                for(unit_t j0=jlb,j1=jlb+1;j0<jub;++j0,++j1)
                {
                    const double y0=y[j0];
                    const double y1=y[j1];
                    for(unit_t i0=ilb,i1=i0+1;i0<iub;++i0,++i1)
                    {
                        // load global field
                        double dmin = (f[1]=d[j0][i0]),      dmax=dmin;
                        dmin =  min_of(f[2]=d[j1][i0],dmin); dmax=max_of(dmax,f[2]);
                        dmin =  min_of(f[3]=d[j1][i1],dmin); dmax=max_of(dmax,f[3]);
                        dmin =  min_of(f[4]=d[j0][i1],dmin); dmax=max_of(dmax,f[4]);

                        // get rid of trivial case
                        if(dmin<zlo||dmax>zhi)
                        {
                            continue; // no possible intersection
                        }
                        const double x0 = x[i0];
                        const double x1 = x[i1];

                        
                        // loop over levels
                        for(size_t k=nc;k>0;--k)
                        {
                            // build the local field
                            const double zk = z[k];
                            g[1] = f[1]-zk;
                            g[2] = f[2]-zk;
                            g[3] = f[3]-zk;
                            g[4] = f[4]-zk;
                            g[0] = 0.25*(g[1]+g[2]+g[3]+g[4]);
                        }
                    }
                }
            }
#endif

        };

    }
}

#endif

