//! \file
#ifndef Y_EUCLIDEAN_POINT_INCLUDED
#define Y_EUCLIDRAN_POINT_INCLUDED 1

#include "y/type/point3d.hpp"
#include "y/type/args.hpp"
#include "y/type/bzset.hpp"
#include "y/ptr/arc.hpp"
#include "y/core/list.hpp"

namespace upsylon {

    namespace math {

        namespace Euclidean {

            template <typename T, template <class> class POINT >
            class Point  : public counted_object
            {
            public:
                Y_DECL_ARGS(T,type);
                typedef POINT<type>     Type;
                static const size_t     Dimensions = sizeof(Type)/sizeof(type);
                typedef arc_ptr<Point>  Pointer;

                Type position;
                Type celerity;

                inline explicit Point() throw() : position(), celerity() {}
                inline explicit Point(const Type p) throw() : position(p), celerity() {}
                inline explicit Point(const Type p, const Type c) throw() : position(p), celerity(c) {}

                inline virtual ~Point() throw()
                {
                    bzset(position);
                    bzset(celerity);
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Point);
            };

            template <typename T,template <class> class POINT >
            class Node : public Point<T,POINT>::Pointer
            {
            public:
                typedef Point<T,POINT>              PointType;
                typedef typename PointType::Pointer SharedPoint;
                typedef typename PointType::Type    Type;
                typedef core::list_of_cpp<Node>     List;

                Node *next;
                Node *prev;

                inline explicit Node( const SharedPoint &P ) throw() : SharedPoint(P), next(0), prev(0) {}
                inline virtual ~Node() throw() {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Node);
            };

        }

    }

}

#endif

