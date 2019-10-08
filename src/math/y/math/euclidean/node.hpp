
//!\file

#ifndef Y_EUCLIDEAN_NODE_INCLUDED
#define Y_EUCLIDEAN_NODE_INCLUDED 1

#include "y/math/euclidean/point.hpp"

namespace upsylon {

    namespace math {

        namespace Euclidean {

            //! key node: node and unique point
            typedef key_address<2> NodeKey;

            template <typename T, template <class> class VTX>
            class Node : public Object, public Point<T,VTX>::Pointer
            {
            public:
                Y_EUCLIDEAN_POINT_TYPES();
                typedef intr_ptr<NodeKey,Node> Pointer;

                const NodeKey uuid;


                inline virtual ~Node() throw() {}
                inline explicit Node(const SharedPoint &p) throw() :
                Object(), SharedPoint(p), uuid(*this,*p) {}
                inline const NodeKey & key() const throw() { return uuid; }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Node);
            };

#define Y_EUCLIDEAN_NODE_TYPES()               \
typedef Node<T,VTX>                NodeType;   \
typedef typename NodeType::Pointer SharedNode


        }
    }
}

#endif
