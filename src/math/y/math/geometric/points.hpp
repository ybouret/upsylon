//! \file
#ifndef Y_MATH_GEOM_POINTS_INCLUDED
#define Y_MATH_GEOM_POINTS_INCLUDED 1

#include "y/math/types.hpp"
#include "y/type/point3d.hpp"
#include "y/sequence/vector.hpp"
#include "y/os/static-check.hpp"

namespace upsylon
{
    namespace math
    {
        namespace Geometric
        {

            template <const size_t DIM,typename T>
            struct InfoFor;

            template <typename T,template <typename> class POINT>
            struct PointInfoFor
            {
                Y_DECL_ARGS(T,type);
                typedef POINT<mutable_type>                         Type;
                static const size_t                                 Dimension = sizeof(Type)/sizeof(type);
                typedef typename InfoFor<Dimension,type>::PointType Core;
                static inline void CheckSizes()
                {
                    Y_STATIC_CHECK(sizeof(Type)==sizeof(Core),PointSize);
                }

            };

            template <typename T> struct InfoFor<2,T>
            {
                typedef point2d<T> PointType;
            };

            template <typename T> struct InfoFor<3,T>
            {
                typedef point3d<T> PointType;
            };

            // utility collection of points
            template <
            typename                  T,
            template <typename> class POINT,
            template <typename> class SEQUENCE = vector_for<memory::global>::device >
            class Points : public SEQUENCE< POINT<T> >
            {
            public:
                Y_DECL_ARGS(T,type);
                typedef PointInfoFor<T,POINT>    PointInfo;
                typedef typename PointInfo::Type PointType;
                typedef typename PointInfo::Core CorePoint;

                inline explicit Points() {}
                inline virtual ~Points() throw() {}


            private:
                Y_DISABLE_ASSIGN(Points);
            };

        }
    }
}

#endif

