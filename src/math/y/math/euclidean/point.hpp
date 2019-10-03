//! \file
#ifndef Y_EUCLIDEAN_POINT_INCLUDED
#define Y_EUCLIDRAN_POINT_INCLUDED 1

#include "y/type/point3d.hpp"
#include "y/type/args.hpp"
#include "y/type/bzset.hpp"
#include "y/ptr/arc.hpp"
#include "y/core/list.hpp"
#include "y/core/node.hpp"

namespace upsylon {

    namespace math {

        namespace Euclidean {


            template <typename T,size_t> struct __VTX;
            template <typename T> struct __VTX<T,2>
            {
                typedef point2d<T> Type;
            };

            template <typename T> struct __VTX<T,3>
            {
                typedef point3d<T> Type;
            };

            typedef counted_object Object;

            template <typename T, template <class> class POINT >
            class Point : public Object
            {
            public:
                Y_DECL_ARGS(T,type);
                typedef POINT<type>     Vertex;
                static const size_t     Dimensions = sizeof(Vertex)/sizeof(type);
                typedef arc_ptr<Point>  Pointer;
                typedef typename __VTX<type,Dimensions>::Type VTX;
                
                Vertex position;

                inline explicit Point() throw() : position() {}
                inline explicit Point(const Vertex p) throw() : position(p){}

                inline virtual ~Point() throw()
                {
                    bzset(position);
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Point);
            };

#define Y_EUCLIDEAN_ARGS()                      \
Y_DECL_ARGS(T,type);                            \
typedef Point<T,POINT>              PointType;  \
typedef typename PointType::Pointer SharedPoint;\
typedef typename PointType::Vertex  Vertex;     \
typedef typename PointType::VTX     VTX

            template <typename T,template <class> class POINT >
            class Node : public Point<T,POINT>::Pointer, public core::inode< Node<T,POINT> >
            {
            public:
                Y_EUCLIDEAN_ARGS();
                typedef core::inode< Node<T,POINT> > iNode;
                typedef core::list_of_cpp<Node>      List;


                inline explicit Node( const SharedPoint &P ) throw() : SharedPoint(P), iNode() {}
                inline virtual ~Node() throw() {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Node);
            };

        }

    }

}

#endif

