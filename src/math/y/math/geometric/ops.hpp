
//! \file
#ifndef Y_MATH_GEOM_OPS_INCLUDED
#define Y_MATH_GEOM_OPS_INCLUDED 1

#include "y/math/geometric/points.hpp"

namespace upsylon
{
    namespace math
    {
        namespace Geometric
        {
            struct Ops
            {
                template <typename T,
                template <typename> class POINT,
                template <typename> class SEQUENCE> static inline
                POINT<T> Barycenter(const SEQUENCE< POINT<T> > &points)
                {
                    typedef  PointInfoFor<T,POINT>    PointInfo;
                    typedef  typename PointInfo::Type PointType;
                    typedef  typename PointInfo::Core CoreType;
                    typedef  SEQUENCE< POINT<T> >     SequenceType;
                    
                    const size_t n   = points.size();
                    CoreType     bar = PointInfo::Origin();
                    switch( n )
                    {
                        case 0:  return PointInfo::Core2Type(bar);
                        case 1:  return points.front();
                        default: break;
                    }
                    typename SequenceType::const_iterator i=points.begin();
                    for(size_t j=n;j>0;++i,--j)
                    {
                        const PointType &p = *i;
                        bar += PointInfo::Type2Core(p);
                    }
                    bar /= n;
                    return PointInfo::Core2Type(bar);
                }
                
                template <typename T,
                template <typename> class POINT,
                template <typename> class SEQUENCE> static inline
                POINT<T> Barycenter(const Points<T,POINT,SEQUENCE> &points)
                {
                    const SEQUENCE< POINT<T> > &self = points;
                    return Barycenter<T,POINT,SEQUENCE>(self);
                }
                
            };
        }
    }
}

#endif

