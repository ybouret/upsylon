
//!\file

#ifndef Y_EUCLIDEAN_NODE_INCLUDED
#define Y_EUCLIDEAN_NODE_INCLUDED 1

#include "y/math/euclidean/point.hpp"
#include "y/math/euclidean/basis.hpp"

namespace upsylon {

    namespace math {

        namespace Euclidean {

            //==================================================================
            //
            //! class of Arc
            //
            //==================================================================
            enum ArcClass
            {
                Arc0,
                Arc1,
                Arc2
            };

            //==================================================================
            //
            //! key node: node and unique point
            //
            //==================================================================
            typedef key_address<2> NodeKey;

            //==================================================================
            //
            //
            //! a node is a shared point with local data
            //
            //
            //==================================================================
            template <typename T, template <class> class VTX>
            class Node : public Object, public Point<T,VTX>::Pointer
            {
            public:
                Y_EUCLIDEAN_POINT_TYPES();                  //!< aliases
                typedef intr_ptr<NodeKey,Node>   Pointer;   //!< alias
                typedef __Basis<type,Dimensions> Basis;     //!< alias

                const_vertex &P;         //!< alias to node position
                const NodeKey uuid;      //!< node+point
                const_vertex  V;         //!< velocity
                const_vertex  A;         //!< acceleration
                const_vertex  dP;        //!< Pnext-P
                const_vertex  dV;        //!< Vnext-V
                const_vertex  dA;        //!< Anext-A
                const_vertex  Q;         //!< expansion vector
                const_vertex  W;         //!< expansion vector
                const Basis   basis;     //!< local basis

                //! cleanup
                inline virtual ~Node() throw() { reset(); }

                //! setup
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

                //! key for database
                inline const NodeKey & key() const throw() { return uuid; }

                //! reset all local data
                inline void reset() throw()
                {
                    zforce(V);
                    zforce(A);
                    zforce(dP);
                    zforce(dV);
                    zforce(dA);
                    zforce(Q);
                    zforce(W);
                    aliasing::_(basis).zero();
                }


                //! initialize basis with tangent and curvature
                inline void initializeBasis() throw()
                {
                    const_type v2 = V.norm2();
                    if(v2>0)
                    {
                        const_type v = sqrt_of(v2);
                        aliasing::_(basis.t)         = V/v;
                        aliasing::_(basis.curvature) = vertex::det(V,A)/(v*v*v);
                    }
                }

                //! method pointer to interpolate data
                typedef void (Node::*Compute)(const_type,vertex *,vertex *,vertex *) const;

                //! set to zero if not null
#define Y_EUCLIDEAN_XZERO(ADDR)       do { if(ADDR) { bzset(*ADDR);    } } while(false)
                //! set to value if not null
#define Y_EUCLIDEAN_XCOPY(ADDR,VALUE) do { if(ADDR) { *(ADDR)=(VALUE); } } while(false)

                //! compute for Arc0
                inline void compute0( const_type u, vertex *p, vertex *dp, vertex *d2p) const throw()
                {

                    Y_EUCLIDEAN_XZERO(d2p);

                    if(dp)
                    {
                        *dp = dP;
                    }

                    if(p) {
                        *p = P + u * dP;
                    }
                }

                //! compute for Arc1
                inline void compute1( const_type u, vertex *p, vertex *dp, vertex *d2p ) const throw()
                {
                    if(d2p)
                    {
                        *d2p = dV + (1-(u+u))*Q;
                    }

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

                //! compute for Arc2
                inline void compute2( const_type u, vertex *p, vertex *dp, vertex *d2p) const throw()
                {
                    if(d2p)
                    {
                        *d2p = A + u * dA + u*(1-u)*(Q+u*W);
                    }

                    const_type u2      = u*u;
                    const_type u3      = u*u2;
                    const_type u4      = u2*u2;
                    const_type u2over2 = u2/2;


                    if(dp)
                    {
                        const_type u3over3 = u3/3;
                        const_type u4over4 = u4/4;
                        *dp = V + u*A + u2over2 * dA + (u2over2-u3over3) * Q + (u3over3-u4over4) * W;
                    }


                    if(p)
                    {
                        const_type u3over6  = u3/6;
                        const_type u4over12 = u4/12;
                        const_type u5over20 = u3*u2/20;
                        *p = P + u*V + u2over2 * A + u3over6 * dA + (u3over6 -u4over12) * Q + (u4over12-u5over20) * W;
                    }

                }



            private:
                Y_DISABLE_COPY_AND_ASSIGN(Node);
            };

            //! nested declarations
#define Y_EUCLIDEAN_NODE_TYPES()               \
Y_EUCLIDEAN_POINT_TYPES();                     \
typedef Euclidean::Node<T,VTX>     NodeType;   \
typedef typename NodeType::Pointer SharedNode; \
typedef typename NodeType::Basis   Basis

        }
    }
}

#endif
