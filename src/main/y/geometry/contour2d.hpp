//! \file
#ifndef Y_GEOM_CONTOUR2D_INCLUDED
#define Y_GEOM_CONTOUR2D_INCLUDED 1

#include "y/type/point2d.hpp"
#include "y/ordered/sorted-vector.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/pooled.hpp"
#include "y/ptr/intr.hpp"
#include "y/associative/set.hpp"

namespace upsylon {
    
    namespace geometry {
        
        struct contour
        {
            enum sign_type
            {
                is_negative,
                is_zero,
                is_positive
            };
            
            static sign_type sign_of( const double value ) throw();
            
            typedef memory::pooled                             allocator;  //!< local memory allocator
            typedef increasing_comparator<double>              comparator; //!< local comparator
            typedef sorted_vector<double,comparator,allocator> levels_type; //!< for levels
            typedef ordered_multiple<levels_type>              levels;
            static double average( const double, const double, const double, const double ) throw();
        };
        
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
        struct contour2d
        {
            
            //! physical position
            typedef point2d<double> vertex;
            
            //! logical location, with lexicographic ordering
            class coordinate
            {
            public:
                static const short full = 0;
                static const short half = 1;
                
                const unit_t i;
                const unit_t j;
                const short  q;
                
                coordinate(const unit_t,const unit_t,const short) throw(); //!< setup
                ~coordinate() throw();                                      //!< cleanup
                coordinate(const coordinate &) throw();                     //!< copy
                
                friend bool operator==(const coordinate&,const coordinate&) throw(); //!< component-wise
                friend bool operator!=(const coordinate&,const coordinate&) throw(); //!< component-wise
                friend bool operator< (const coordinate&,const coordinate&) throw(); //!< lexicographic
                static int  compare(const coordinate&,const coordinate&) throw();    //!< lexicographic
                friend std::ostream & operator<<(std::ostream &,const coordinate&);  //!< display
                
                void __run( hashing::function & ) const throw();
                void __sto( unit_t *a ) const throw();
                
            private:
                Y_DISABLE_ASSIGN(coordinate);
            };
            
            //! logical edge: a set of one or two points
            class edge
            {
            public:
                const coordinate lower; //!< lower by lexicographic
                const coordinate upper; //!< upper by lexicographic
                ~edge() throw();
                edge(const edge &) throw();
                edge(const coordinate &single) throw();
                edge(const coordinate &a, const coordinate &b) throw(); //!< a!=b
                
                friend bool operator==(const edge&,const edge&) throw();
                friend bool operator!=(const edge&,const edge&) throw();
                
                class hasher
                {
                public:
                    hashing::fnv H;
                    hasher() throw();
                    ~hasher() throw();
                    size_t operator()( const edge & ) throw();
                    
                private:
                    Y_DISABLE_COPY_AND_ASSIGN(hasher);
                };
                
            private:
                Y_DISABLE_ASSIGN(edge);
            };
            
            //! unique point
            class point_ : public counted_object
            {
            public:
                const edge   location; //!< logical
                const vertex position; //!< physical
                
                explicit point_(const edge &l, const vertex p) throw();
                virtual ~point_() throw();
                
                const edge & key() const throw();
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(point_);
            };
            
            typedef intr_ptr<edge,point_>                       point;
            typedef set<edge,point,edge::hasher,memory::global> points;

            
            class segment : public object
            {
            public:
               
                
                const point head;
                const point tail;
                segment    *next;
                segment    *prev;
                
                typedef core::list_of_cpp<segment> list_type;
                
                explicit segment( const point &h, const point &t) throw();
                virtual ~segment() throw();
                
                
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(segment);
            };
            
            class segments : public segment::list_type
            {
            public:
                explicit segments() throw();
                virtual ~segments() throw();
            private:
                Y_DISABLE_COPY_AND_ASSIGN(segments);
            };
            
            class level_ : public counted, public points
            {
            public:
                const size_t index;
                segments     seg;
                
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(level_);
            };
            
            typedef intr_ptr<size_t,level_> level;
            
            
            
            
            
            
            
            struct context
            {
                const double     *d;
                const coordinate *c;
                const vertex     *v;
            };
            
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
            void scan(const FIELD           &data,
                      const unit_t           ilb,
                      const unit_t           iub,
                      const unit_t           jlb,
                      const unit_t           jub,
                      const ARRAY           &x,
                      const ARRAY           &y,
                      const contour::levels &z )
            {
                
                const size_t nc = z.size();
                //createDB(db,nc);
                if(nc<=0) return;
                
                const double zmin = z.head();
                const double zmax = z.tail();
                
                const unit_t jlbp1 = jlb+1;
                const unit_t ilbp1 = ilb+1;
                
                
                for(unit_t j=jlb,jp1=jlbp1;j<jub;++j,++jp1)
                {
                    const double y0 = double(y[j]);
                    const double y1 = double(y[jp1]);
                    const double ym = 0.5*(y0+y1);
                    
                    for(unit_t i=ilb,ip1=ilbp1;i<iub;++i,++ip1)
                    {
                        //------------------------------------------------------
                        //
                        // outer loop: scan the square
                        //
                        //------------------------------------------------------
                        
                        
                        //------------------------------------------------------
                        // collect local values
                        //------------------------------------------------------
                        const double d1 = data[j  ][i];    double dmin=d1, dmax=d1;
                        const double d2 = data[j  ][ip1];  dmin = min_of(dmin,d2); dmax=max_of(dmax,d2);
                        const double d3 = data[jp1][ip1];  dmin = min_of(dmin,d3); dmax=max_of(dmax,d3);
                        const double d4 = data[jp1][i  ];  dmin = min_of(dmin,d4); dmax=max_of(dmax,d4);
                        
                        if(dmax<zmin||dmin>zmax)
                        {
                            continue; // no intersection in this square
                        }
                        
                        const double global_d[5] =
                        {
                            contour::average(d1,d2,d3,d4),
                            d1,d2,d3,d4
                        };
                        
                        const coordinate c[5] =
                        {
                            coordinate(i,  j,   coordinate::half),
                            coordinate(i,  j,   coordinate::full),
                            coordinate(ip1,j,   coordinate::full),
                            coordinate(ip1,jp1, coordinate::full),
                            coordinate(i,  jp1, coordinate::full)
                        };
                        
                        const double x0 = x[i];
                        const double x1 = x[ip1];
                        const vertex v[5] =
                        {
                            vertex(0.5*(x0+x1),ym),
                            vertex(x0,y0),
                            vertex(x1,y0),
                            vertex(x1,y1),
                            vertex(x0,y1)
                        };
                        
                        //------------------------------------------------------
                        //
                        // inner loop: over the levels
                        //
                        //------------------------------------------------------
                        for(size_t k=nc;k>0;--k)
                        {
                            //--------------------------------------------------
                            // build the local field values to test against 0
                            //--------------------------------------------------
                            const double z_k = z[k];
                            if(z_k>dmax||z_k<dmin) continue; // no possible intersection
                            const double d[5] =
                            {
                                global_d[0]-z_k,
                                global_d[1]-z_k,
                                global_d[2]-z_k,
                                global_d[3]-z_k,
                                global_d[4]-z_k,
                            };
                            
                            const context ctx = { d,c,v };
                            (void)ctx;
                            
                            //--------------------------------------------------
                            // scan the triangles
                            //--------------------------------------------------
#if 0
                            scan_triangles(db,
                                           k,
                                           c,
                                           d,
                                           v);
#endif
                        }
                        
                        
                    }
                }
                
            }
            
        private:
            
        };
        
    }
}

#endif

