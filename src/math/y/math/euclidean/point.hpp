//!\file

#ifndef Y_EUCLIDEAN_POINT_INCLUDED
#define Y_EUCLIDEAN_POINT_INCLUDED 1

#include "y/type/point3d.hpp"
#include "y/type/args.hpp"
#include "y/type/bzset.hpp"
#include "y/container/key-address.hpp"
#include "y/ptr/intr.hpp"
#include "y/type/aliasing.hpp"
#include "y/ios/ostream.hpp"

namespace upsylon {

    namespace math {

        namespace Euclidean {

            //==================================================================
            //
            //! base object
            //
            //==================================================================
            typedef counted_object Object;

            //==================================================================
            //
            //! unique point key
            //
            //==================================================================
            typedef key_address<1> PointKey;

            //==================================================================
            //
            //
            //! point that can be shared between multiple nodes
            //
            //
            //==================================================================
            template <typename T, template <class> class VTX>
            class Point : public Object
            {
            public:
                Y_DECL_ARGS(T,type);                              //!< aliases
                typedef VTX<type>                vertex;          //!< alias
                typedef const vertex             const_vertex;    //!< alias
                static const size_t              Dimensions = sizeof(vertex)/sizeof(type); //!< [2|3]
                typedef intr_ptr<PointKey,Point> Pointer;         //!< alias

                vertex         position; //!< physical position
                const PointKey uuid;     //!< unique value

                //! setup
                inline explicit Point() throw() : position(), uuid( *this ) {}

                //! cleanup
                inline virtual ~Point() throw() { bzset_(position); }

                //! setup from vertex
                inline explicit Point(const_vertex p) throw() : position(p), uuid( *this ) {}

                //! key for database
                inline const PointKey & key() const throw() { return uuid; }

                //! helper to display
                static inline ios::ostream & Print( ios::ostream &os, const_vertex &v )
                {
                    const_type *p = (const_type *)&v;
                    os("%.15g",p[0]); for(size_t dim=1;dim<Dimensions;++dim) os(" %1.5g",p[dim]);
                    return os;
                }

                static inline
                type Curvature( const_vertex &V, const vertex &A ) throw()
                {
                    const_type V2 = V.norm2();
                    if(V2<=0)
                    {
                        return 0;
                    }
                    else
                    {
                        const_type v = sqrt_of(V2);
                        return vertex::det(V,A)/(v*v*v);
                    }
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Point);
            };

            //! nested types forwarding
#define Y_EUCLIDEAN_POINT_TYPES()             \
Y_DECL_ARGS(T,type);                                  \
typedef             Point<T,VTX>            PointType;\
typedef typename    PointType::vertex       vertex;   \
typedef typename    PointType::const_vertex const_vertex;\
typedef typename    PointType::Pointer      SharedPoint;\
static const size_t Dimensions =            PointType::Dimensions

        }

    }

}

#endif

