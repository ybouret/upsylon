//! \file

#ifndef Y_MATH_K_MEANS_INCLUDED
#define Y_MATH_K_MEANS_INCLUDED 1

#include "y/type/point3d.hpp"
#include "y/sort/heap.hpp"
#include "y/comparison.hpp"
#include "y/container/sequence.hpp"
#include "y/type/block/zset.hpp"

namespace upsylon {
    
    namespace mkl {
        
        namespace kernel {
            
            //! generic centroid
            template <size_t DIM>
            class centroid
            {
            public:
                //--------------------------------------------------------------
                //
                // members
                //
                //--------------------------------------------------------------
                double r[DIM]; //!< position
                double g[DIM]; //!< barycenter
                size_t count;  //!< number of owned points
                
                //--------------------------------------------------------------
                //
                // C++
                //
                //--------------------------------------------------------------
                
                //! setup
                inline  centroid() throw() : r(), g(), count(0)
                {
                    for(size_t dim=0;dim<DIM;++dim)
                    {
                        r[dim]=0;
                        g[dim]=0;
                    }
                }
                
                //! setup with v[0..DIM-1]
                template <typename T>
                inline  centroid(const T *v) throw() : r(), g(), count(0) {
                    assert(v!=NULL);
                    for(size_t dim=0;dim<DIM;++dim)
                    {
                        r[dim]=v[dim];
                        g[dim]=0;
                    }
                }
                
                //! cleanup
                inline ~centroid() throw() {}
                
                //! copy
                inline  centroid(const centroid &_) throw() : r(), g(), count(_.count)
                {
                    for(size_t dim=0;dim<DIM;++dim)
                    {
                        r[dim]=_.r[dim];
                        g[dim]=_.g[dim];
                    }
                }
                
                //--------------------------------------------------------------
                //
                // methods
                //
                //--------------------------------------------------------------
                
                //! assign v[0..DIM-1]
                template <typename T>
                inline void set(const T *v) throw()
                {
                    assert(v!=NULL);
                    for(size_t dim=0;dim<DIM;++dim)
                    {
                        r[dim]=v[dim];
                    }
                }
                
                
                //! g=0
                inline void g_reset() throw()
                {
                    for(size_t dim=0;dim<DIM;++dim)
                    {
                        g[dim] = 0;
                    }
                    count = 0;
                }
                
                //! g += v, ++count
                template <typename T>
                inline void g_update(const T *v) throw()
                {
                    for(size_t dim=0;dim<DIM;++dim)
                    {
                        g[dim] += double(v[dim]);
                    }
                    ++count;
                }
                
                //! g+= n*v, count += n
                template <typename T>
                inline void g_update(const T *v, const size_t n) throw()
                {
                    for(size_t dim=0;dim<DIM;++dim)
                    {
                        g[dim] += n * double(v[dim]);
                    }
                    count += n;
                }
                
              
                //! squared distance from r to v
                template <typename T>
                double d2(const T *v) const throw()
                {
                    assert(v);
                    double sq[DIM];
                    for(size_t dim=0;dim<DIM;++dim)
                    {
                        sq[dim] = square_of(r[dim]-double(v[dim]));
                    }
                    hsort(sq,DIM,comparison::increasing<double>);
                    double ans = sq[0];
                    for(size_t dim=1;dim<DIM;++dim)
                    {
                        ans += sq[dim];
                    }
                    return ans;
                }
                
                //! display
                inline friend std::ostream & operator<<( std::ostream &os, const centroid &c)
                {
                    os << '{';
                    os << c.r[0];
                    for(size_t i=1;i<DIM;++i) os << ' ' << c.r[i];
                    os << '}';
                    return os;
                }
                
            private:
                Y_DISABLE_ASSIGN(centroid);
            };
            
        }
        
        //! k-means algorithm
        template <typename T,typename VERTEX>
        struct k_means
        {
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------
            static  const size_t                 dimensions = sizeof(VERTEX)/sizeof(T); //!< the dimensions
            typedef kernel::centroid<dimensions> centroid;                              //!< alias
           
            //------------------------------------------------------------------
            //
            // functions
            //
            //------------------------------------------------------------------
            
            //! find closest centroid
            static inline size_t find_closest(const accessible<centroid> &centroids,
                                              const VERTEX               &v) throw()
            {
                assert(centroids.size()>0);
                const size_t nc = centroids.size();
                size_t       ic = 1;
                double       d2 = centroids[1].d2((const T*)&v);
                for(size_t i=2;i<=nc;++i)
                {
                    const double tmp = centroids[i].d2((const T*)&v);
                    if(tmp<d2)
                    {
                        ic=i;
                        d2=tmp;
                    }
                }
                return ic;
            }
            
            //! initialize all barycenters and counts
            static inline void initialize( addressable<centroid> &centroids ) throw()
            {
                for(size_t i=centroids.size();i>0;--i)
                {
                    centroids[i].g_reset();
                }
            }
            
            //! build indices, return true if achieved, i.e. same indices
            static inline bool built(addressable<centroid>      &centroids,
                                     const accessible<VERTEX>   &vertices,
                                     addressable<size_t>        &indices) throw()
            {
                assert(vertices.size()==indices.size());
                assert(centroids.size()>0);
                
                initialize(centroids);
                bool achieved = true;
                for(size_t i=vertices.size();i>0;--i)
                {
                    const VERTEX &v = vertices[i];
                    const size_t  j = find_closest(centroids,v);
                    size_t       &J = indices[i];
                    if( j!=J )
                    {
                        achieved = false;
                        J        = j;
                    }
                    centroids[j].g_update( (const T*)&v );
                }
                return achieved;
            }
            
            //! build indices, return true if achieved, i.e. same indices
            static inline bool built(addressable<centroid>      &centroids,
                                     const accessible<VERTEX>   &vertices,
                                     addressable<size_t>        &indices,
                                     const accessible<size_t>   &weights) throw()
            {
                assert(vertices.size()==indices.size());
                assert(vertices.size()==weights.size());
                assert(centroids.size()>0);
                
                initialize(centroids);
                bool achieved = true;
                for(size_t i=vertices.size();i>0;--i)
                {
                    const VERTEX &v = vertices[i];
                    const size_t  j = find_closest(centroids,v);
                    size_t       &J = indices[i];
                    if( j!=J )
                    {
                        achieved = false;
                        J        = j;
                    }
                    centroids[j].g_update( (const T*)&v, weights[i] );
                }
#ifndef NDEBUG
                size_t w = 0; for(size_t i=weights.size();i>0;--i) w += weights[i];
                size_t W = 0; for(size_t i=centroids.size();i>0;--i) W += centroids[i].count;
                assert(w==W);
#endif
                return achieved;
            }
            
            //! update positions, return false if a centroid is empty
            static inline bool update(addressable<centroid> &centroids)
            {
                bool success = true;
                assert(centroids.size()>0);
                for(size_t j=centroids.size();j>0;--j)
                {
                    centroid    &c = centroids[j];
                    const size_t n = c.count;
                    if(n<=0)
                    {
                        success = false;
                        for(size_t i=0;i<dimensions;++i) c.g[i] = c.r[i];
                    }
                    else
                    {
                        for(size_t i=0;i<dimensions;++i) c.r[i] = (c.g[i]/=n);
                    }
                }
                return success;
            }
            
            
        };
        
    }
}


#endif


