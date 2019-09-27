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

        //======================================================================
        //
        //! helper for any contour
        //
        //======================================================================
        struct contour
        {
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------

            typedef memory::pooled                             allocator;   //!< local memory allocator
            typedef increasing_comparator<double>              comparator;  //!< local comparator
            typedef sorted_vector<double,comparator,allocator> levels_type; //!< for levels
            typedef ordered_multiple<levels_type>              levels;      //!< set of multiple ordered values

            //! machine precision sign
            enum sign_type
            {
                is_negative, //!< value<0
                is_zero,     //!< value==0
                is_positive  //!< value>0
            };

            //------------------------------------------------------------------
            //
            // methods
            //
            //------------------------------------------------------------------

            //! convert value to sign_type
            static sign_type sign_of( const double value ) throw();

            //! get the most precise 4-average
            static double average( const double, const double, const double, const double ) throw();
        };

        //======================================================================
        //
        //! finding iso levels in 2d
        //
        //======================================================================
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
            //__________________________________________________________________
            //
            //! physical position
            //__________________________________________________________________
            typedef point2d<double> vertex;

            //__________________________________________________________________
            //
            //! logical location, with lexicographic ordering
            //__________________________________________________________________
            class coordinate
            {
            public:
                static const short full = 0; //!< mark for complete coordinate
                static const short half = 1; //!< mark for center coordinate
                
                const unit_t i; //!< logical index
                const unit_t j; //!< logical index
                const short  q; //!< modifier
                
                coordinate(const unit_t,const unit_t,const short) throw();           //!< setup
                ~coordinate() throw();                                               //!< cleanup
                coordinate(const coordinate &) throw();                              //!< copy
                friend bool operator==(const coordinate&,const coordinate&) throw(); //!< component-wise
                friend bool operator!=(const coordinate&,const coordinate&) throw(); //!< component-wise
                friend bool operator< (const coordinate&,const coordinate&) throw(); //!< lexicographic
                static int  compare(const coordinate&,const coordinate&) throw();    //!< lexicographic
                void        __run(hashing::function &) const throw();                //!< run hash function
                void        __sto(unit_t *) const throw();                           //!< store components
                friend std::ostream & operator<<(std::ostream &,const coordinate&);  //!< display

            private:
                Y_DISABLE_ASSIGN(coordinate);
            };

            //__________________________________________________________________
            //
            //! logical edge: a set of (one or) two points, acting as a key
            //__________________________________________________________________
            class edge
            {
            public:
                const coordinate lower; //!< lower by lexicographic
                const coordinate upper; //!< upper by lexicographic

                ~edge() throw();                                        //!< cleanup
                edge(const edge &) throw();                             //!< copy
                edge(const coordinate &single) throw();                 //!< lower=upper=single
                edge(const coordinate &a, const coordinate &b) throw(); //!< a!=b, then ordered
                
                friend bool operator==(const edge&,const edge&) throw(); //!< testing same values
                friend bool operator!=(const edge&,const edge&) throw(); //!< testing different values

                //! dedicated hasher
                class hasher
                {
                public:
                    hashing::fnv H;    //!< hashing function
                    hasher() throw();  //!< setup
                    ~hasher() throw(); //!< cleanup

                    //! get hash code
                    size_t operator()( const edge & ) throw();
                    
                private:
                    Y_DISABLE_COPY_AND_ASSIGN(hasher);
                };
                
            private:
                Y_DISABLE_ASSIGN(edge);
            };

            //__________________________________________________________________
            //
            //! unique point
            //__________________________________________________________________
            class point_ : public counted_object
            {
            public:
                const edge   location; //!< logical
                const vertex position; //!< physical
                
                explicit     point_(const edge &,const vertex &) throw(); //!< setup
                virtual     ~point_() throw();                            //!< cleanup
                const edge & key() const throw();                         //!< for set<...>
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(point_);
            };

            //__________________________________________________________________
            //
            //! alias for dynamic point
            //__________________________________________________________________
            typedef intr_ptr<edge,point_>                       point;


            class isopoint : public point
            {
            public:
                typedef core::list_of_cpp<isopoint> list_type;
                isopoint *next;
                isopoint *prev;

                explicit isopoint( const point & ) throw();
                virtual ~isopoint() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(isopoint);
            };

            class isoline_ : public isopoint::list_type, public counted_object
            {
            public:
                explicit isoline_() throw();
                virtual ~isoline_() throw();
                
                const bool cyclic;
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(isoline_);
            };

            typedef arc_ptr<isoline_>                  isoline;
            typedef vector<isoline,contour::allocator> isolines;

            //__________________________________________________________________
            //
            //! a segment as a node for a list
            //__________________________________________________________________
            class segment : public object
            {
            public:
                const point head; //!< head point
                const point tail; //!< tail point
                segment    *next; //!< for list
                segment    *prev; //!< for list

                typedef core::list_of_cpp<segment> list_type; //!< base class for segments

                explicit segment(const point &h, const point &t) throw(); //!< setup
                virtual ~segment() throw();                               //!< cleanup

                static bool are_the_same(const segment &, const segment &) throw(); //!< test
                static bool are_opposite(const segment &, const segment &) throw(); //!< test
                friend bool operator==(const segment &, const segment &) throw();   //!< are_the_same or are_opposite
                friend bool operator!=(const segment &, const segment &) throw();   //!< test if different
            private:
                Y_DISABLE_COPY_AND_ASSIGN(segment);
            };

            //__________________________________________________________________
            //
            //! a list of segments
            //__________________________________________________________________
            class segments : public segment::list_type
            {
            public:
                explicit segments() throw(); //!< setup
                virtual ~segments() throw(); //!< cleanup

                //! build all curves from this segment
                void build( isolines &iso ) const;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(segments);
            };


            //__________________________________________________________________
            //
            //! base class for a level
            //__________________________________________________________________
            typedef set<edge,point,edge::hasher,memory::global> points;

            //__________________________________________________________________
            //
            //! a level is a set of points, has segments and optional curves
            //__________________________________________________________________

            class level_ : public counted, public points
            {
            public:
                const size_t     index; //!< index in the contour::level
                segments         slist; //!< list of associated segments
                isolines         iso;   //!< sequence of isolines
                
                explicit         level_(const size_t ) throw(); //!< setup
                virtual         ~level_() throw();              //!< cleanup
                const size_t   & key() const throw();           //!< key for level_set
                void             compute_iso();                 //!< compile to iso
                
                //! check consistency
                void check() const;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(level_);
                friend struct contour2d;
                //! create/query a single point
                point_ *single(const coordinate &c, const vertex &v );
                
                
                //! create/query a point on the edge(ca,cb)
                point_ *couple(const coordinate &ca, const vertex &va, const double da,
                               const coordinate &cb, const vertex &vb, const double db);
                
                //! make segment from two single points
                void    full(const coordinate &ca, const vertex &va,
                             const coordinate &cb, const vertex &vb);
                
                //! make segment from zero vertex and intersection, da*db<0
                void    inter1(const coordinate &cz, const vertex &vz,
                               const coordinate &ca, const vertex &va, const double da,
                               const coordinate &cb, const vertex &vb, const double db);
                
                //! make segment from two intersections: ds*da<0, ds*db<0,
                void    inter2(const coordinate &cs, const vertex &vs, const double ds,
                               const coordinate &ca, const vertex &va, const double da,
                               const coordinate &cb, const vertex &vb, const double db);
            };

            //__________________________________________________________________
            //
            //! dynamic level
            //__________________________________________________________________
            typedef intr_ptr<size_t,level_>                         level;

            //__________________________________________________________________
            //
            //! base class for level_set
            //__________________________________________________________________
            typedef set<size_t,level,key_dumper,contour::allocator> level_set_;

            //__________________________________________________________________
            //
            //! a database of levels
            //__________________________________________________________________
            class level_set : public level_set_
            {
            public:
                explicit level_set() throw();    //!< setup
                virtual ~level_set() throw();    //!< cleanup
                void     create(const size_t n); //!< create [1:n] levels
                void     check_all() const;
                void     compute_isolines();
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(level_set);
            };
            
            
            //__________________________________________________________________
            //
            //!lightweight context for internal scanning
            //__________________________________________________________________
            struct context
            {
                const double     *d; //!< [0:4] values
                const coordinate *c; //!< [0:4] values
                const vertex     *v; //!< [0:4] values
                level_           *l; //!< target level
            };

            //__________________________________________________________________
            //
            //! a low-level contour algorithm
            //__________________________________________________________________
            /**
             data            ! matrix/field of data to contour
             ilb,iub,jlb,jub ! index bounds of data matrix[j][i]
             x               ! data matrix column coordinates
             y               ! data matrix row coordinates
             z               ! contour levels
             */
            template<
            typename FIELD,
            typename ARRAY
            > static inline
            void scan(level_set             &ls,
                      const FIELD           &data,
                      const unit_t           ilb,
                      const unit_t           iub,
                      const unit_t           jlb,
                      const unit_t           jub,
                      const ARRAY           &x,
                      const ARRAY           &y,
                      const contour::levels &z )
            {
                
                const size_t nc = z.size();
                ls.create(nc);
                if(nc<=0) return;
                
                const double zmin  = z.head(), zmax  = z.tail(); assert(zmin<=zmax);
                const unit_t jlbp1 = jlb+1,    ilbp1 = ilb+1;
                

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
                            continue; // no possible intersection in this square
                        }
                        
                        const double global_d[5] = { contour::average(d1,d2,d3,d4),d1,d2,d3,d4 };
                        
                        const coordinate c[5] =
                        {
                            coordinate(i,  j,   coordinate::half),
                            coordinate(i,  j,   coordinate::full),
                            coordinate(ip1,j,   coordinate::full),
                            coordinate(ip1,jp1, coordinate::full),
                            coordinate(i,  jp1, coordinate::full)
                        };
                        
                        const double x0   = double(x[i]);
                        const double x1   = double(x[ip1]);
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
                        level_set::iterator it=ls.begin();
                        for(size_t k=1;k<=nc;++k,++it)
                        {
                            assert((**it).index==k);
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
                            context ctx = { d,c,v,&(**it) };

                            //--------------------------------------------------
                            // scan the triangles
                            //--------------------------------------------------
                            scan_triangles(ctx);
                        }
                        
                    }
                }
#if !defined(NDEBUG)
                ls.check_all();
#endif
            }
            
        private:
            static void   scan_triangles(context &);
            

        };
        
    }
}

#endif

