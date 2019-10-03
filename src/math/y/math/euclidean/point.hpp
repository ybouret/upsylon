//! \file
#ifndef Y_EUCLIDEAN_POINT_INCLUDED
#define Y_EUCLIDEAN_POINT_INCLUDED 1

#include "y/type/point3d.hpp"
#include "y/type/args.hpp"
#include "y/type/bzset.hpp"
#include "y/ptr/arc.hpp"
#include "y/core/list.hpp"
#include "y/core/node.hpp"

namespace upsylon {

    namespace math {

        namespace Euclidean {

            //! helper to find the right point[2|3]d<T>
            template <typename T,size_t> struct __VTX;

            //! map to point2d<T>
            template <typename T> struct __VTX<T,2>
            {
                typedef point2d<T> Type; //!< alias
            };

            //! map to point3d<T>
            template <typename T> struct __VTX<T,3>
            {
                typedef point3d<T> Type; //!< alias
            };

            //! base class for object
            typedef counted_object Object;


            //==================================================================
            //
            //
            //! a point has a position
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
                typedef arc_ptr<Point>                        Pointer;        //!< for shared point
                typedef typename __VTX<type,Dimensions>::Type VTX;            //!< mapping point type

                //==============================================================
                //
                // members
                //
                //==============================================================
                Vertex position;

                //==============================================================
                //
                // methods
                //
                //==============================================================
                inline explicit Point() throw() : position() {}               //!< setup with default position
                inline explicit Point(const Vertex p) throw() : position(p){} //!< setup
                inline virtual ~Point() throw() { bzset(position); }          //!< cleanup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Point);
            };

            //! forwarding type
#define Y_EUCLIDEAN_ARGS()                       \
Y_DECL_ARGS(T,type);                             \
typedef Point<T,POINT>              PointType;   \
typedef typename PointType::Pointer SharedPoint; \
typedef typename PointType::Vertex  Vertex;      \
typedef typename PointType::VTX     VTX

            //==================================================================
            //
            //
            //! a dynamic node, which is a shared point with local data
            //
            //
            //==================================================================
            template <typename T,template <class> class POINT >
            class Node : public Point<T,POINT>::Pointer, public core::inode< Node<T,POINT> >
            {
            public:
                //==============================================================
                //
                // types and declarations
                //
                //==============================================================
                Y_EUCLIDEAN_ARGS();                         //!< aliases
                typedef core::inode< Node<T,POINT> > iNode; //!< alias
                typedef core::list_of_cpp<Node>      List;  //!< base type for list


                //==============================================================
                //
                // members
                //
                //==============================================================
                Vertex celerity;

                //==============================================================
                //
                // methods
                //
                //==============================================================
                //! setup
                inline explicit Node( const SharedPoint &P ) throw() : SharedPoint(P), iNode(), celerity() {}
                //! cleanup
                inline virtual ~Node() throw() {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Node);
            };

        }

    }

}

#endif

