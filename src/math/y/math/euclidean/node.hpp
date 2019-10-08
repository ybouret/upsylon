
//!\file

#ifndef Y_EUCLIDEAN_NODE_INCLUDED
#define Y_EUCLIDEAN_NODE_INCLUDED 1

#include "y/math/euclidean/point.hpp"
#include "y/math/euclidean/basis.hpp"

namespace upsylon {

    namespace math {

        namespace Euclidean {

            enum ArcClass
            {
                Arc0,
                Arc1,
                Arc2
            };

            //! key node: node and unique point
            typedef key_address<2> NodeKey;

            template <typename T, template <class> class VTX>
            class Node : public Object, public Point<T,VTX>::Pointer
            {
            public:
                Y_EUCLIDEAN_POINT_TYPES();
                typedef intr_ptr<NodeKey,Node>   Pointer;
                typedef __Basis<type,Dimensions> Basis;

                const_vertex &P;
                const NodeKey uuid;
                const_vertex  V;
                const_vertex  A;
                const_vertex  dP;
                const_vertex  dV;
                const_vertex  dA;
                const Basis   basis;

                inline virtual ~Node() throw() {}
                inline explicit Node(const SharedPoint &p) throw() :
                Object(),
                SharedPoint(p),
                P( p->position ),
                uuid(*this,*p),
                V(),
                A(),
                dP(),
                dV(),
                dA(),
                basis()
                {}

                inline const NodeKey & key() const throw() { return uuid; }

                inline void reset() throw()
                {
                    bzset_(V);
                    bzset_(A);
                    bzset_(dP);
                    bzset_(dV);
                    bzset_(dA);
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Node);
            };

#define Y_EUCLIDEAN_NODE_TYPES()               \
Y_EUCLIDEAN_POINT_TYPES();\
typedef Node<T,VTX>                NodeType;   \
typedef typename NodeType::Pointer SharedNode


        }
    }
}

#endif
