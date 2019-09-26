//! \file
#ifndef Y_GEOMETRY_ISO2D_INCLUDED
#define Y_GEOMETRY_ISO2D_INCLUDED 1


#include "y/type/point2d.hpp"
#include "y/associative/set.hpp"
#include "y/ptr/arc.hpp"
#include "y/ptr/intr.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/pooled.hpp"

namespace upsylon {

    namespace geometry {

        //! finding iso levels in 2d
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
         on an edge, which defines a set of unique Coordinates
         for any point of the isoline
         */
        struct Iso2D
        {
            typedef point2d<double> Vertex;      //!< alias
            typedef memory::pooled  Allocator;   //!< for some levels, medium sized
            static const unit_t     Full = 0x00; //!< for full Coordinate
            static const unit_t     Half = 0x01; //!< for half Coordinate

            //__________________________________________________________________
            //
            //! Logical Coordinate
            //__________________________________________________________________
            class Coordinate
            {
            public:
                const unit_t   i; //!< i index of the data
                const unit_t   j; //!< j index if the data
                const unit_t   q; //!< full|half

                Coordinate(const unit_t ii, const unit_t jj, const unsigned qq) throw(); //!< setup
                ~Coordinate() throw();                                                   //!< destructor
                Coordinate(const Coordinate &other) throw();                             //!< copy
                friend bool operator==(const Coordinate &,const Coordinate &) throw();   //!< test equality
                static int compare(const Coordinate &,const Coordinate &) throw();       //!< lexicographic comparison
                void run( hashing::function &H ) const throw();                          //!< update hash function value

            private:
                Y_DISABLE_ASSIGN(Coordinate);
            };

            //__________________________________________________________________
            //
            //! ordered pair of Coordinates -> unique key
            //__________________________________________________________________
            class EdgeLabel
            {
            public:
                const Coordinate lower; //!< lexicographic lower
                const Coordinate upper; //!< lexicographic upper

                //! lower=upper=only
                EdgeLabel( const Coordinate &only ) throw();

                //! setup and order
                EdgeLabel( const Coordinate &a, const Coordinate &b) throw() ;

                //! copy
                EdgeLabel(const EdgeLabel &other) throw();

                //! update hash function
                void run( hashing::function &H ) const throw();

                //! equality of lower and upper for both
                friend bool operator==(const EdgeLabel &lhs, const EdgeLabel &rhs) throw();

            private:
                Y_DISABLE_ASSIGN(EdgeLabel);

            public:
                //! specialized hasher
                class Hasher
                {
                public:
                    hashing::fnv H;     //!< internal hash functon
                    Hasher() throw();   //!< setup
                    ~Hasher() throw();  //!< destruct
                    size_t operator()(const EdgeLabel &) throw(); //!< compute hash key

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Hasher);
                };
            };

            //__________________________________________________________________
            //
            //! a unique point is a logical location and a physical position
            //__________________________________________________________________
            class UniquePoint : public counted_object
            {
            public:
                const EdgeLabel  tag; //!< logical  location
                const Vertex     vtx; //!< physical position on this edge

                //!setup
                explicit UniquePoint(const EdgeLabel &id, const Vertex &v) throw();

                //! desctructor
                virtual ~UniquePoint() throw();

                //! get the key for set
                const EdgeLabel & key() const throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(UniquePoint);
            };


            typedef intr_ptr<EdgeLabel,UniquePoint>                SharedPoint;  //!< alias
            typedef set<EdgeLabel,SharedPoint,EdgeLabel::Hasher>   UniquePoints; //!< alias

            //! a segment connecting two shared points
            class Segment : public object
            {
            public:
                Segment          *next; //!< for list
                Segment          *prev; //!< for list
                const SharedPoint a;    //!< point from an iso-level
                const SharedPoint b;    //!< point from an iso-level

                //! setup
                explicit Segment(UniquePoint *pa, UniquePoint *pb) throw() ;

                //! dersctructor
                virtual ~Segment() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Segment);
            };

            typedef core::list_of_cpp<Segment> Segments; //!< alias: list of Segments


            //! return the position where the field vanished, va*vb<0
            static Vertex   zfind(const Vertex &pa, const double va,
                                  const Vertex &pb, const double vb) throw();

            //! database of points/segments for a given level value
            class SharedPoints : public UniquePoints, public counted
            {
            public:
                explicit SharedPoints() throw(); //!< setup
                virtual ~SharedPoints() throw(); //!< destructor

                Segments segments; //!< segments built from unique points

                //! get one point or create it from fallback
                UniquePoint * operator()(const Coordinate &c, const Vertex &fallback);


                //! get one point or create it from two points
                UniquePoint * operator()(const Coordinate &c0, const Vertex &p0, const double v0,
                                         const Coordinate &c1, const Vertex &p1, const double v1);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(SharedPoints);
            };

            typedef arc_ptr<SharedPoints>   Level;   //!< alias
            typedef vector<Level,Allocator> Levels;  //!< alias



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

            //! a low-level contour algorithm
            /**
             data            !  matrix/field of data to contour
             ilb,iub,jlb,jub ! index bounds of data matrix[j][i]
             x               ! data matrix column Coordinates
             y               ! data matrix row Coordinates
             z               ! contour levels in INCREASING order, nc=z.size()
             */
            template<
            typename FIELD,
            typename ARRAY
            > static inline
            void Scan(Levels              &levels,
                      const FIELD         &data,
                      const unit_t         ilb,
                      const unit_t         iub,
                      const unit_t         jlb,
                      const unit_t         jub,
                      const ARRAY         &x,
                      const ARRAY         &y,
                      const array<double> &z )
            {


                const size_t nc = z.size();
#if !defined(NDEBUG)
                for(size_t i=1;i<nc;++i)
                {
                    assert(z[i]<=z[i+1]);
                }
#endif
                if(nc<=0)
                {
                    levels.free();
                    return;
                }

                //--------------------------------------------------------------
                //
                // prepare levels
                //
                //--------------------------------------------------------------
                levels.ensure(nc);
                while(levels.size()>nc)
                {
                    levels.pop_back();
                }
                while(levels.size()<nc)
                {
                    const Level _ = new SharedPoints();
                    levels.push_back( _ );
                }
                assert(nc==levels.size());
                for(size_t k=nc;k>0;--k)
                {
                    levels[k]->free();
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
                        // global Coordinates
                        //------------------------------------------------------
                        const Vertex vtx[5] =
                        {
                            Vertex(0.5*(x0+x1),ymid),
                            Vertex(x0,y0),
                            Vertex(x1,y0),
                            Vertex(x1,y1),
                            Vertex(x0,y1)
                        };

                        const Coordinate coord[5] =
                        {
                            Coordinate(i0,j0,Half),
                            Coordinate(i0,j0,Full),
                            Coordinate(i1,j0,Full),
                            Coordinate(i1,j1,Full),
                            Coordinate(i0,j1,Full)
                        };

                        //------------------------------------------------------
                        // loop over levels
                        //------------------------------------------------------
                        scan_triangles(levels, g, vtx, coord, z);
                    }

                }


            }

        private:
            static void scan_triangles(Levels             &lvl,
                                       const double       *g,
                                       const Vertex       *vtx,
                                       const Coordinate   *coord,
                                       const array<double> &z);



        public:
            //! a linked shared_point
            class Point : public object, public SharedPoint
            {
            public:

                Point *next; //!< for list
                Point *prev; //!< for list

                //! setup
                Point(const SharedPoint &p) throw();

                //! destructor
                virtual ~Point() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Point);
            };

            typedef core::list_of_cpp<Point> Points; //!< alias

            //! a list of points
            class Line : public object, public Points
            {
            public:
                typedef core::list_of_cpp<Line> List; //!< alias
                Line *next; //!< for list
                Line *prev; //!< for list

                explicit Line() throw(); //!< setup
                virtual ~Line() throw(); //!< setup

                //! push back to vertices, with free() before
                void compile_to( sequence<Vertex> &vertices ) const;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Line);
            };

            //! a set of lines from a set of segments
            class Lines : public Line::List
            {
            public:
                explicit Lines() throw();
                virtual ~Lines() throw();

                //! convert segments into connected line
                void connect( const Segments &segments );

                //! generic conversion to a sequence of smart pointer to vertices
                template <typename CURVE_PTR>
                inline void compile_to( sequence<CURVE_PTR> &curves ) const
                {
                    curves.free();
                    curves.ensure(size);
                    for(const Line *l=head;l;l=l->next)
                    {
                        typename CURVE_PTR::mutable_type *c = new typename CURVE_PTR::mutable_type();
                        {
                            const CURVE_PTR ptr(c);
                            curves.push_back(ptr);
                        }
                        l->compile_to(*c);
                    }
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Lines);
            };

            typedef vector<Vertex> CurveType; //!< base class for curve

            //! a curve, to be pointee
            class Curve : public CurveType, public counted
            {
            public:
                typedef arc_ptr<Curve> Pointer; //!< alias for dynamic curves
                explicit Curve() throw();       //!< setup
                virtual ~Curve() throw();       //!< destructor

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Curve);
            };

            typedef vector<Curve::Pointer,Allocator> CurvesType; //!< base class for curves

            //! a collection of curves for the same value
            class Curves : public CurvesType, public counted
            {
            public:
                typedef arc_ptr<Curves> Pointer; //!< alias for dynamic level set
                explicit Curves() throw();       //!< setup
                virtual ~Curves() throw();       //!< desctructor

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Curves);
            };

            typedef vector<Curves::Pointer,Allocator> LevelSet; //!< alias

            //! convert a database of different levels into a level_set
            static void convert(LevelSet     &output,
                                const Levels &input);

            //! a low-level contour algorithm
            /**
             data            !  matrix/field of data to contour
             ilb,iub,jlb,jub ! index bounds of data matrix[j][i]
             x               ! data matrix column Coordinates
             y               ! data matrix row Coordinates
             z               ! contour levels in INCREASING order, nc=z.size()
             */
            template<
            typename FIELD,
            typename ARRAY
            > static inline
            void Build(LevelSet           &ls,
                       const FIELD        &data,
                       const unit_t        ilb,
                       const unit_t        iub,
                       const unit_t        jlb,
                       const unit_t        jub,
                       const ARRAY         &x,
                       const ARRAY         &y,
                       const array<double> &z
                       )
            {
                // first pass: builds temporary levels
                Levels levels;
                Scan(levels, data, ilb, iub, jlb, jub, x, y, z);
                convert(ls,levels);
            }

        };

    }
}

#endif

