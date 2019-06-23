//! \file
#ifndef Y_MATH_GEOM_POINTS_INCLUDED
#define Y_MATH_GEOM_POINTS_INCLUDED 1

#include "y/math/types.hpp"
#include "y/type/point3d.hpp"
#include "y/sequence/vector.hpp"

namespace upsylon
{
    namespace math
    {
        namespace Geometric
        {

            //! generic boundaries for operations
            enum Boundaries
            {
                Standard, //!< for standard curve
                Periodic  //!< for periodic curve
            };

            //! throw exception if value<=0
            template <typename T> T CheckNorm2(const T value, const char *ctx);

            //! contains dedicated types
            template <const size_t DIM,typename T> struct InfoFor;

            //! handle point[2|3]d<T> and POINT<T>
            template <typename T,template <typename> class POINT>
            struct PointInfoFor
            {
                Y_DECL_ARGS(T,type);                                                           //!< alias
                typedef POINT<type>                     Type;                                  //!< user point type
                static const size_t                     Dimension = sizeof(Type)/sizeof(type); //!< the point dimension
                typedef InfoFor<Dimension,mutable_type> InfoType;                              //!< info for this topolgy
                typedef typename InfoType::PointType    Core;                                  //!< internal point[2|3]d<T>
                
                static inline Core       & Type2Core( Type       &t ) throw() { return *(Core *)&t;       } //!< POINT -> point[2|3]
                static inline const Core & Type2Core( const Type &t ) throw() { return *(const Core *)&t; } //!< POINT -> point[2|3], const
                static inline Type &       Core2Type( Core       &c ) throw() { return *(Type *)&c;       } //!< point[2|3] -> POINT
                static inline const Type & Core2Type( const Core &c ) throw() { return *(const Type *)&c; } //!< point[2|3] -> POINT, const
                static inline Core         Origin()                   throw() { return Core();            } //!< [(0,0)|(0,0,0)]

            };

            //! 2D types
            template <typename T> struct InfoFor<2,T>
            {
                typedef point2d<T> PointType; //!< core point type
            };

            //! 3D type
            template <typename T> struct InfoFor<3,T>
            {
                typedef point3d<T> PointType; //!< core point type
            };
            
            //! utility collection of points
            template <
            typename                  T,
            template <typename> class POINT,
            template <typename> class SEQUENCE = vector_for<memory::global>::device >
            class Points : public SEQUENCE< POINT<T> >
            {
            public:
                Y_DECL_ARGS(T,type);                                            //!< aliases
                typedef PointInfoFor<T,POINT>                 PointInfo;        //!< alias for point
                typedef typename PointInfo::Type              PointType;        //!< POINT<T>
                typedef typename PointInfo::Core              CorePoint;        //!< point[2|3]d<T>
                typedef SEQUENCE<PointType>                   SequenceType;     //!< alias for SEQUENCE
                typedef typename SequenceType::iterator       iterator;         //!< iterator alias
                typedef typename SequenceType::const_iterator const_iterator;   //!< const iterator alias

                inline explicit Points() {}                                                            //!< setup
                inline explicit Points(const size_t n, const as_capacity_t &_) : SequenceType(n,_) {}  //!< setup with capacity
                inline virtual ~Points() throw() {}                                                    //!< cleanup
                inline Points( const Points &other ) : SequenceType(other) {}                          //!< copy

                //! add a point with two coordinates
                inline Points & add(param_type x, param_type y)               { const PointType p(x,y);   this->push_back(p); return *this; }

                //! add a point with three coordinates
                inline Points & add(param_type x, param_type y, param_type z) { const PointType p(x,y,z); this->push_back(p); return *this; }
                
                
            private:
                Y_DISABLE_ASSIGN(Points);

            };
            
        }
    }
}

#endif

