//! \file
#ifndef Y_EUCLIDEAN_POINT_INCLUDED
#define Y_EUCLIDEAN_POINT_INCLUDED 1

#include "y/math/euclidean/types.hpp"
#include "y/container/key-address.hpp"
#include "y/ptr/intr.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/bzset.hpp"

namespace upsylon {

    namespace math {

        namespace Euclidean {
            
            //==================================================================
            //
            //! key for unique allocated point
            //
            //==================================================================
            typedef key_address<1> PointKey;

            //==================================================================
            //
            //
            //! a point has a position and a UUID
            //
            //
            //==================================================================
            template <typename T, template <class> class POINT >
            class Point : public Object
            {
            public:
                //==============================================================
                //
                // types and declarations
                //
                //==============================================================
                Y_DECL_ARGS(T,type);                                          //!< alias
                typedef POINT<type>                           Vertex;         //!< the data handling vertex
                static const size_t Dimensions = sizeof(Vertex)/sizeof(type); //!< dimensions
                typedef  __Core<type,Dimensions>              Core;           //!< alias
                typedef typename Core::Type                   VTX;            //!< mapping point type
                typedef intr_ptr<PointKey,Point>              Pointer;        //!< for shared point

                //==============================================================
                //
                // members
                //
                //==============================================================
                Vertex         position; //!< current position
                const PointKey uuid;     //!< uuid based on address

                //==============================================================
                //
                // methods
                //
                //==============================================================
                inline explicit Point() throw() : position(), uuid(*this) {}                //!< setup with default position
                inline explicit Point(const Vertex p) throw() : position(p), uuid(*this) {} //!< setup
                inline virtual ~Point() throw() { bzset(position); }                        //!< cleanup
                inline const PointKey & key() const throw() { return uuid; }                //!< for database

                //! output by mapping to point<>
                static inline ios::ostream & Print( ios::ostream &fp, const Vertex &v)
                {
                    return Core::Print(fp,aliasing::cast<VTX,Vertex>(v));
                }

                //! casting to point[2|3]d
                inline VTX       & operator*()       throw() { return aliasing::cast<VTX,Vertex>(position); }

                //! casting to point[2|3]d
                inline const VTX & operator*() const throw() { return aliasing::cast<VTX,Vertex>(position); }

                
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Point);
            };

            //==================================================================
            //
            //! forwarding point types
            //
            //==================================================================
#define Y_EUCLIDEAN_POINT_ARGS()                  \
Y_DECL_ARGS(T,type);                              \
typedef Point<T,POINT>               PointType;   \
static const size_t Dimensions =     PointType::Dimensions; \
typedef typename PointType::Pointer  SharedPoint;  \
typedef typename PointType::Vertex   Vertex;      \
typedef typename PointType::VTX      VTX

           
            
        }

    }

}

#endif

