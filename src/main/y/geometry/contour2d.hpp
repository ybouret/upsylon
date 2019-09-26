//! \file
#ifndef Y_GEOM_CONTOUR2D_INCLUDED
#define Y_GEOM_CONTOUR2D_INCLUDED 1

#include "y/type/point2d.hpp"
#include "y/ordered/sorted-vector.hpp"
#include "y/memory/pooled.hpp"

namespace upsylon {

    namespace geometry {

        struct contour
        {
            enum sign_type
            {
                is_neg,
                is_zero,
                is_pos
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
                static const unit_t full = 0;
                static const unit_t half = 1;

                const unit_t i;
                const unit_t j;
                const unit_t q;

                coordinate(const unit_t,const unit_t,const unit_t) throw(); //!< setup
                ~coordinate() throw();                                      //!< cleanup
                coordinate(const coordinate &) throw();                     //!< copy

                friend bool operator==(const coordinate&,const coordinate&) throw(); //!< component-wise
                friend bool operator!=(const coordinate&,const coordinate&) throw(); //!< component-wise
                friend bool operator< (const coordinate&,const coordinate&) throw(); //!< lexicographic
                static int  compare(const coordinate&,const coordinate&) throw();    //!< lexicographic
                friend std::ostream & operator<<(std::ostream &,const coordinate&);  //!< display

            private:
                Y_DISABLE_ASSIGN(coordinate);
            };

            //! logical edge
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

            private:
                Y_DISABLE_ASSIGN(edge);
            };

            class point : public counted_object
            {
            public:
                const edge   location; //!< logical
                const vertex position; //!< physical

                explicit point(const edge &l, const vertex p) throw();
                virtual ~point() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(point);
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
                if(nc<=0) return;
                const double zmin = z.head();
                const double zmax = z.tail();

                const unit_t jlbp1 = jlb+1;
                for(unit_t i=ilb,ip1=ilb+1;i<iub;++i,++ip1)
                {
                    for(unit_t j=jlb,jp1=jlbp1;j<jub;++j,++jp1)
                    {

                        //------------------------------------------------------
                        //
                        // outer loop: scan the square
                        //
                        //------------------------------------------------------


                        //------------------------------------------------------
                        // collect local values
                        //------------------------------------------------------
                        const double v1 = data[i  ][j];    double vmin=v1, vmax=v1;
                        const double v2 = data[ip1][j];    vmin = min_of(vmin,v2); vmax=max_of(vmax,v2);
                        const double v3 = data[ip1][jp1];  vmin = min_of(vmin,v3); vmax=max_of(vmax,v3);
                        const double v4 = data[i  ][jp1];  vmin = min_of(vmin,v4); vmax=max_of(vmax,v4);

                        const double v[5] =
                        {
                            contour::average(v1,v2,v3,v4),
                            v1,v2,v3,v4
                        };

                        const coordinate c[5] =
                        {
                            coordinate(i,  j,   coordinate::half),
                            coordinate(i,  j,   coordinate::full),
                            coordinate(ip1,j,   coordinate::full),
                            coordinate(ip1,jp1, coordinate::full),
                            coordinate(i,  jp1, coordinate::full)
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
                            if(z_k>vmax||z_k<vmin) continue; // no possible intersection

                            const double d[5] =
                            {
                                v[0]-z_k, v[1]-z_k, v[2]-z_k, v[3]-z_k, v[4]-z_k,
                            };

                            //--------------------------------------------------
                            // scan the triangles
                            //--------------------------------------------------
                            scan_triangles(c, d,contour::sign_of(d[0]) );
                        }


                    }
                }

            }

            static inline void scan_triangles(const coordinate         *c,
                                              const double             *d,
                                               const contour::sign_type s0)
            {
                static const unsigned indices[4][2] =
                {
                    {1,2}, {2,3}, {3,4}, {4,1}
                };
                for(size_t t=0;t<4;++t)
                {
                    scan_triangle(c, d, s0, indices[t][0], indices[t][1] );
                }
            }


            static inline void scan_triangle(const coordinate        *c,
                                             const double            *d,
                                             const contour::sign_type s0,
                                             const unsigned           i1,
                                             const unsigned           i2)
            {
                assert(i1!=i2);
                assert(i1>=1&&i1<=4);
                assert(i2>=1&&i2<=4);
                const double d0 = d[0];  assert(s0==contour::sign_of(d0));
                const double d1 = d[i1];
                const double s1 = contour::sign_type( d1 );
                const double d2 = d[i2];
                const double s2 = contour::sign_type( d2 );

                // study the different cases
                switch(0)
                {

                    default:
                        break;
                }

            }
        };

    }
}

#endif

