//!\file

#ifndef Y_EUCLIDEAN_POINT_INCLUDED
#define Y_EUCLIDEAN_POINT_INCLUDED 1

#include "y/type/point3d.hpp"
#include "y/type/args.hpp"
#include "y/type/bzset.hpp"
#include "y/container/key-address.hpp"
#include "y/ptr/intr.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {

    namespace math {

        namespace Euclidean {


            typedef counted_object Object;
            typedef key_address<1> PointKey;

            //! point that can be shared between multiple nodes
            template <typename T, template <class> class VTX>
            class Point : public Object
            {
            public:
                Y_DECL_ARGS(T,type);
                typedef VTX<type>                vertex;
                typedef const vertex             const_vertex;
                static const size_t              Dimensions = sizeof(vertex)/sizeof(type);
                typedef intr_ptr<PointKey,Point> Pointer;

                vertex         r;
                const PointKey uuid;

                inline explicit Point() throw() : r(), uuid( *this ) {}
                inline virtual ~Point() throw() { bzset_(r); }
                inline explicit Point(const_vertex p) throw() : r(p), uuid( *this ) {}
                inline const PointKey & key() const throw() { return uuid; }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Point);
            };


#define Y_EUCLIDEAN_POINT_TYPES()\
Y_DECL_ARGS(T,type);\
typedef             Point<T,VTX>            PointType;\
typedef typename    PointType::vertex       vertex;\
typedef typename    PointType::const_vertex const_vertex;\
typedef typename    PointType::Pointer      SharedPoint;\
static const size_t Dimensions =             PointType::Dimensions

        }

    }

}

#endif

