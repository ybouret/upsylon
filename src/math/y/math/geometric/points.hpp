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

            //! generic boundaries for operations
            enum Boundaries
            {
                Standard,
                Periodic
            };

            //! contains dedicated types
            template <const size_t DIM,typename T> struct InfoFor;

            //! handle point[2|3]d<T> and POINT<T>
            template <typename T,template <typename> class POINT>
            struct PointInfoFor
            {
                Y_DECL_ARGS(T,type);
                typedef POINT<type>                     Type;                                  //!< user point type
                static const size_t                     Dimension = sizeof(Type)/sizeof(type); //!< the point dimension
                typedef InfoFor<Dimension,mutable_type> InfoType;                              //!< info for this topolgy
                typedef typename InfoType::PointType    Core;                                  //!< internal point[2|3]d<T>
                
                static inline Core &Type2Core(  Type &t ) throw()
                {
                    return *(Core *)&t;
                }
                
                static inline const Core &Type2Core( const Type &t ) throw()
                {
                    return *(const Core *)&t;
                }
                
                static inline Type & Core2Type( Core &c ) throw()
                {
                    return *(Type *)&c;
                }
                
                static inline const Type & Core2Type( const Core &c ) throw()
                {
                    return *(const Type *)&c;
                }
                
                static inline Core Origin() throw()
                {
                    return Core();
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
                typedef PointInfoFor<T,POINT>                 PointInfo;
                typedef typename PointInfo::Type              PointType;
                typedef typename PointInfo::Core              CorePoint;
                typedef SEQUENCE<PointType>                   SequenceType;
                typedef typename SequenceType::iterator       iterator;
                typedef typename SequenceType::const_iterator const_iterator;
                
                
                inline explicit Points() {}
                inline explicit Points(const size_t n, const as_capacity_t &_) : SequenceType(n,_) {}
                inline virtual ~Points() throw() {}
                inline Points( const Points &other ) : SequenceType(other) {}

                inline Points & add(param_type x, param_type y)               { const PointType p(x,y);   this->push_back(p); return *this; }
                inline Points & add(param_type x, param_type y, param_type z) { const PointType p(x,y,z); this->push_back(p); return *this; }
                
                
            private:
                Y_DISABLE_ASSIGN(Points);

            };
            
        }
    }
}

#endif

