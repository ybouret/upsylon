//! \file
#ifndef Y_GEOMETRY_CONTOUR2D_INCLUDED
#define Y_GEOMETRY_CONTOUR2D_INCLUDED 1

#include "y/type/point2d.hpp"
#include "y/ptr/intr.hpp"
#include "y/ptr/counted.hpp"
#include "y/associative/set.hpp"

namespace upsylon
{
    namespace geometry
    {

        //! data structure for 2d contour scanning
        struct contour2d
        {

            class identifier
            {
            public:
                const unit_t   i;
                const unit_t   j;
                const unsigned p; //! on 5 bits

                explicit identifier(const unit_t ii, const unit_t jj, const unsigned pp) throw() : i(ii), j(jj), p(pp) {}

                virtual ~identifier() throw() {}

                identifier(const identifier &other) throw() : i(other.i), j(other.j), p(other.p) {}

                friend bool operator==( const identifier &lhs, const identifier &rhs) throw()
                {
                    return lhs.i == rhs.i && lhs.j == rhs.j && lhs.p == rhs.p;
                }

                typedef hashing::fnv hashing_function;
                class hasher
                {
                public:
                    hashing_function H;
                    inline hasher() throw() : H() {}
                    inline ~hasher() throw() {}
                    inline size_t operator()( const identifier &id ) throw()
                    {
                        H.set();
                        H.run_type(id.i);
                        H.run_type(id.j);
                        H.run_type(id.p);
                        return H.key<size_t>();
                    }

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(hasher);
                };

            private:
                Y_DISABLE_ASSIGN(identifier);
            };

            typedef point2d<double>     point;      //!< point physical coordinate

            class unique_point : public counted_object
            {
            public:
                const identifier id;
                const point      r;
                explicit unique_point(const identifier &who, const point &coord ) throw() : id(who), r(coord) {}
                virtual ~unique_point() throw() {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(unique_point);
            };



            typedef intr_ptr<identifier,unique_point>               shared_point;

            class segment : public object
            {
            public:
                segment *next;
                segment *prev;

                const shared_point a;
                const shared_point b;

                segment( const shared_point &A, const shared_point &B) throw() :
                next(0), prev(0), a(A), b(B) {}
                virtual ~segment() throw() {}

                typedef core::list_of_cpp<segment> list;


            private:
                Y_DISABLE_COPY_AND_ASSIGN(segment);
            };


            typedef set<identifier,shared_point,identifier::hasher> shared_points; //!< amongst one level

            class shared_points_db : public shared_points, public counted
            {
            public:
                const size_t   index; //!< index in levels
                const size_t & key() const throw() { return index; }

                segment::list segments;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(shared_points_db);
            };


            //! callback upon a found segment
            typedef void              (*callback)(double x0, double y0, double x1, double y1, size_t idx, double lvl, void *args);

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

            //! wrapper for callback
            class wrapper
            {
            public:
                size_t   index; //!< current index
                double   value; //!< current value
                callback proc;  //!< user callback
                void    *args;  //!< callback argument

                //! setup
                inline wrapper(callback _proc,void *_args) : index(0), value(0), proc(_proc), args(_args)
                {
                    if(!proc) proc=nope;
                }

                //! cleanup
                inline ~wrapper() throw() {}

                //! call operator
                inline void operator()( const point a, const point b )
                {
                    proc(a.x,a.y,b.x,b.y,index,value,args);
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(wrapper);
                static inline void nope(double,double,double,double,size_t,double,void*) throw()
                {
                }
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
            void scan(const FIELD         &data,
                      const unit_t         ilb,
                      const unit_t         iub,
                      const unit_t         jlb,
                      const unit_t         jub,
                      const ARRAY         &x,
                      const ARRAY         &y,
                      const array<double> &z,
                      callback             proc,
                      void                *args
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

                wrapper cb(proc,args);

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
                        const point vtx[5] =
                        {
                            point(0.5*(x0+x1),ymid),
                            point(x0,y0),
                            point(x1,y0),
                            point(x1,y1),
                            point(x0,y1)
                        };


                        //------------------------------------------------------
                        // loop over levels
                        //------------------------------------------------------
                        for(size_t k=nc;k>0;--k)
                        {
                            const double zk = (cb.value=z[cb.index=k]);
                            f[1] = g[1] - zk;
                            f[2] = g[2] - zk;
                            f[3] = g[3] - zk;
                            f[4] = g[4] - zk;
                            f[0] = 0.25*(f[1]+f[2]+f[3]+f[4]);

                            //--------------------------------------------------
                            // loop over triangles
                            //--------------------------------------------------
                            static const size_t m0   = 0;
                            const point         p0   = vtx[m0];
                            const double        f0   = f[m0];
                            const unsigned      s0   = (sign_flag(f0) << sign_shift0);
                            for(size_t l=0;l<4;++l)
                            {
                                //----------------------------------------------
                                // get indices
                                //----------------------------------------------
                                const size_t        m1 = tri[l][0];
                                const size_t        m2 = tri[l][1];
                                
                                //----------------------------------------------
                                // get real coordinates
                                //----------------------------------------------
                                const point    p1    = vtx[m1];
                                const point    p2    = vtx[m2];

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
                                        // intercepting
                                    case neg0|pos1|pos2:
                                    case pos0|neg1|neg2:
                                        cb(zfind(p0,f0,p1,f1),
                                           zfind(p0,f0,p2,f2));
                                        break;

                                    case neg0|pos1|neg2:
                                    case pos0|neg1|pos2:
                                        cb( zfind(p0,f0,p1,f1), zfind(p1,f1,p2,f2) );
                                        break;

                                    case neg0|neg1|pos2:
                                    case pos0|pos1|neg2:
                                        cb( zfind(p0,f0,p2,f2), zfind(p1,f1,p2,f2) );
                                        break;

                                        // vertex+intercep
                                    case zzz0|pos1|neg2:
                                    case zzz0|neg1|pos2:
                                        cb(p0,zfind(p1,f1,p2,f2));
                                        break;

                                    case pos0|zzz1|neg2:
                                    case neg0|zzz1|pos2:
                                        cb(p1,zfind(p0,f0,p2,f2));
                                        break;

                                    case pos0|neg1|zzz2:
                                    case neg0|pos1|zzz2:
                                        cb(p2,zfind(p1,f1,p2,f2));
                                        break;

                                        // one of the vertices
                                    case zzz0|zzz1|pos2:
                                    case zzz0|zzz1|neg2:
                                        cb(p0,p1);
                                        break;

                                    case neg0|zzz1|zzz2:
                                    case pos0|zzz1|zzz2:
                                        cb(p1,p2);
                                        break;

                                    case zzz0|neg1|zzz2:
                                    case zzz0|pos1|zzz2:
                                        cb(p0,p2);
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

        private:
            static point    zfind(const point &pa, const double va,
                                  const point &pb, const double vb) throw();

        public:
#if 0
            //! a basic segment
            class segment : public object
            {
            public:
                segment    *next; //!< for list
                segment    *prev; //!< for list
                const point a;    //!< one end
                const point b;    //!< other end

                explicit segment(const point &_, const point &__) throw(); //!< setup
                virtual ~segment() throw();                                //!< cleanup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(segment);
            };

            //! alias to use segments
            typedef core::list_of_cpp<segment> segments;

            //! a level set is a list of segments
            class level_set : public counted_object, public segments
            {
            public:
                typedef intr_ptr<size_t,level_set> pointer;  //!< alias
                typedef set<size_t,pointer>        database; //!< alias

                const size_t index;                  //!< from original guess, is the key
                const size_t  & key() const throw(); //!< for database

                explicit level_set(const size_t idx) throw(); //!< setup
                virtual ~level_set() throw();                 //!< cleanup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(level_set);
            };

            //! database of level sets
            class level_sets : public level_set::database
            {
            public:

                explicit level_sets(const size_t n=0); //!< setup
                virtual ~level_sets() throw();         //!< cleanup

                //! get level set with on the fly creation
                level_set & operator[](const size_t idx);

                //! access, must exists
                const level_set & operator[](const size_t idx) const;

                //! for callback
                static void call(double,double,double,double,size_t,double,void*);

                //! cleanup and scan field, with level ordering
                template< typename FIELD, typename ARRAY>  inline
                void build(const FIELD         &data,
                           const unit_t         ilb,
                           const unit_t         iub,
                           const unit_t         jlb,
                           const unit_t         jub,
                           const ARRAY         &x,
                           const ARRAY         &y,
                           const array<double> &z)
                {
                    free();
                    ensure( z.size() );
                    scan(data, ilb, iub, jlb, jub, x, y, z, call, this);
                    sort_keys( comparison::increasing<size_t> );
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(level_sets);

            };
#endif

        };

    }
}

#endif

