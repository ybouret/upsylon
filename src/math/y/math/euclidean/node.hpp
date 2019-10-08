
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
                const_vertex  Q;
                const_vertex  W;
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
                Q(),
                W(),
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
                    bzset_(Q);
                    bzset_(W);
                    aliasing::_(basis).zero();
                }

                //! set tangent after kinematics
                void setTangent() throw()
                {
                    const_type V2 = V.norm2();
                    if(V2>0)
                    {
                        aliasing::_(basis.t) = V/sqrt_of(V2);
                    }
                }

                typedef void (Node::*Compute)(const_type,vertex *,vertex *,vertex *) const;


#define Y_EUCLIDEAN_XZERO(ADDR)       do { if(ADDR) { bzset(*ADDR);    } } while(false)
#define Y_EUCLIDEAN_XCOPY(ADDR,VALUE) do { if(ADDR) { *(ADDR)=(VALUE); } } while(false)

                inline void compute0( const_type u, vertex *p, vertex *dp, vertex *d2p) const throw()
                {

                    Y_EUCLIDEAN_XZERO(dp);
                    Y_EUCLIDEAN_XZERO(d2p);
                    if(p) {
                        *p = P + u * dP;
                    }
                }

                inline void compute1( const_type u, vertex *p, vertex *dp, vertex *d2p ) const throw()
                {
                    Y_EUCLIDEAN_XZERO(d2p);

                    if(dp)
                    {
                        *dp = V + u * dV + u*(1-u)*Q;
                    }

                    if(p)
                    {
                        const_type u2      = u*u;
                        const_type u3      = u*u2;
                        const_type u2over2 = u2/2;
                        const_type u3over3 = u3/3;
                        *p = P + u * V + u2over2 * dV + (u2over2-u3over3) * Q;
                    }
                }

                inline void compute2( const_type u, vertex *p, vertex *dp, vertex *d2p) const throw()
                {
                    if(d2p)
                    {
                        *d2p = A + u * dA + u*(1-u)*(Q+u*W);
                    }

                    const_type u2 = u*u;
                    const_type u3 = u*u2;
                    const_type u4 = u2*u2;
                    const_type u2over2 = u2/2;
                    const_type u3over3 = u3/3;
                    const_type u4over4 = u4/3;

                    if(dp)
                    {
                        *dp = V;
                    }

                    if(p)
                    {
                        *p = P;
                    }


                }



            private:
                Y_DISABLE_COPY_AND_ASSIGN(Node);
            };

#define Y_EUCLIDEAN_NODE_TYPES()               \
Y_EUCLIDEAN_POINT_TYPES();\
typedef Node<T,VTX>                NodeType;   \
typedef typename NodeType::Pointer SharedNode; \
typedef typename NodeType::Basis   Basis

        }
    }
}

#endif
