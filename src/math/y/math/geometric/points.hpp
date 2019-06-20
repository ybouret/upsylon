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

            template <const size_t DIM,typename T> struct InfoFor;

            template <typename T,template <typename> class POINT>
            struct PointInfoFor
            {
                Y_DECL_ARGS(T,type);
                typedef POINT<mutable_type>             Type;                                  //!< user point type
                static const size_t                     Dimension = sizeof(Type)/sizeof(type); //!< the point dimension
                typedef InfoFor<Dimension,mutable_type> InfoType;                              //!< info for this topolgy
                typedef typename InfoType::PointType    Core;                                  //!< internal representation

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


                inline explicit Points() : origin() {}
                inline virtual ~Points() throw() {}

                inline Points & add(param_type x, param_type y)               { const PointType p(x,y);   this->push_back(p); return *this; }
                inline Points & add(param_type x, param_type y, param_type z) { const PointType p(x,y,z); this->push_back(p); return *this; }

                inline PointType barycenter() const
                {
                    const size_t n = this->size();
                    switch(n)
                    {
                        case 0:  return PointInfo::Core2Type(origin);
                        case 1:  return this->front();
                        default: break;
                    }
                    CorePoint      bar;
                    const_iterator i=this->begin();
                    for(size_t count=n;count>0;--count,++i)
                    {
                        bar += PointInfo::Type2Core(*i);
                    }
                    bar /= n;
                    return PointInfo::Core2Type(bar);
                }

            private:
                Y_DISABLE_ASSIGN(Points);
                
            public:
                const CorePoint origin;
            };

        }
    }
}

#endif

