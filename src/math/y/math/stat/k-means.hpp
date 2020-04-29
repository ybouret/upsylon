//! \file

#ifndef Y_MATH_K_MEANS_INCLUDED
#define Y_MATH_K_MEANS_INCLUDED 1

#include "y/type/point3d.hpp"
#include "y/sort/network/sort3.hpp"
#include "y/comparison.hpp"
#include "y/container/sequence.hpp"
#include "y/type/block/zset.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon {
    
    namespace math {
        
        namespace kernel
        {
            template <size_t DIM> struct real_vertex;
            
            
            template <> struct real_vertex<1>
            {
                typedef double type;
                template <typename T> static inline
                double d2(const double *b, const T *a) throw()
                {
                    return square_of( double(a[0]) -  (b[0]) );
                }
                
                
                
            };
            
            template <>           struct real_vertex<2>
            {
                typedef point2d<double> type;
                template <typename T> static inline
                double d2(const double *b, const T *a) throw()
                {
                    return
                    square_of( double(a[0]) -  (b[0]) ) +
                    square_of( double(a[1]) -  (b[1]) );
                }
                
            };
            
            template <> struct real_vertex<3>
            {
                typedef point3d<double> type;
                template <typename T> static inline
                double d2(const double *b, const T *a) throw()
                {
                    double sq[3] = {
                        square_of( double(a[0]) -  (b[0]) ),
                        square_of( double(a[1]) -  (b[1]) ),
                        square_of( double(a[2]) -  (b[2]) )
                    };
                    network_sort<3>::on(sq, comparison::increasing<double> );
                    return sq[0]+sq[1]+sq[2];
                }
                
                
            };
        }
        
        
        
        template <typename T,typename VERTEX>
        struct k_means
        {
            static  const size_t                    dimensions = sizeof(VERTEX)/sizeof(T);
            typedef kernel::real_vertex<dimensions> ops;
            typedef typename ops::type              vertex;
            
            
            class centroid_ : public counted_object
            {
            public:
                vertex  v;
                vertex  g;
                size_t  count;
                
                inline  centroid_(const VERTEX ini) throw() :
                v(), g(v), count(0)
                {
                    double  *p = (double *)&v;
                    const T *q = (const T *)&ini;
                    for(size_t i=0;i<dimensions;++i) p[i] = double( q[i] );
                    g=v;
                }
                inline virtual ~centroid_() throw() {}
                
                inline friend std::ostream & operator<<( std::ostream &os, const centroid_ &c)
                {
                    os << '{' << c.v << '}';
                    return os;
                }
                
                inline void set() throw()
                {
                    bzset(g);
                    count = 0;
                }
                
                inline void run(const VERTEX &V) throw()
                {
                    double  *tgt = (double  *)&g;
                    const T *src = (const T *)&V;
                    for(size_t i=0;i<dimensions;++i) tgt[i] += double(src[i]);
                    ++count;
                }
                
                inline void run(const VERTEX &V, const size_t n) throw()
                {
                    assert(n>0);
                    double  *tgt = (double  *)&g;
                    const T *src = (const T *)&V;
                    for(size_t i=0;i<dimensions;++i) tgt[i] += n*double(src[i]);
                    count += n;
                }
                
                inline double d2(const VERTEX &V) const throw()
                {
                    return ops::template d2<T>( (const double*)&v,(const T *)&V);
                }
                
                Y_DISABLE_COPY_AND_ASSIGN(centroid_);
            };
            
            typedef arc_ptr<centroid_> centroid;
            
            static inline size_t find_closest(const accessible<centroid> &centroids,
                                              const VERTEX               &v) throw()
            {
                assert(centroids.size()>0);
                const size_t nc = centroids.size();
                size_t       ic = 1;
                double       d2 = centroids[1]->d2(v);
                 for(size_t i=2;i<=nc;++i)
                {
                    const double tmp = centroids[i]->d2(v);
                    if(tmp<d2)
                    {
                        ic=i;
                        d2=tmp;
                    }
                }
                return ic;
            }
            
            
            
           
            
            static inline void set_centroids( addressable<centroid> &centroids ) throw()
            {
                for(size_t i=centroids.size();i>0;--i)
                {
                    centroids[i]->set();
                }
            }
            
            
            static inline bool update(sequence<centroid>          &centroids,
                                      const accessible<VERTEX>    &vertices,
                                      const accessible<size_t>    &indices) throw()
            {
                assert(centroids.size()>0);
                assert(vertices.size() >0);
                
                assert(vertices.size()==indices.size());
                const size_t nc = centroids.size();
                const size_t nv = vertices.size();
                
                
                set_centroids(centroids);
                
                for(size_t j=nv;j>0;--j)
                {
                    const size_t  k   = indices[j];  assert(k>=1); assert(k<=nc);
                    centroids[k]->run( vertices[j] );
                }
                
                bool success = true;
                for(size_t i=nc;i>0;--i)
                {
                    centroid &c = centroids[i];
                    if(c->count<=0)
                    {
                        success = false;
                    }
                    else
                    {
                        c->g /= c->count;
                        c->v = c->g;
                    }
                }
                return success;
            }
            
            
            
            
            
            
        };
        
    }
}


#endif


