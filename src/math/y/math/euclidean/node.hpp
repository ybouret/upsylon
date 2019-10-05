//! \file

#ifndef Y_EUCLIDEAN_NODE_INCLUDED
#define Y_EUCLIDEAN_NODE_INCLUDED 1

#include "y/math/euclidean/point.hpp"

namespace upsylon {

    namespace math {

        namespace Euclidean {

            //==================================================================
            //
            //! key for node database/uuid
            //
            //==================================================================
            typedef key_address<2> NodeKey;

            //==================================================================
            //
            //
            //! a node has a point and local data
            //
            //
            //==================================================================
            template <typename T,template <class> class POINT>
            class Node : public Object
            {
            public:
                //==============================================================
                //
                // types and declarations
                //
                //==============================================================
                Y_EUCLIDEAN_POINT_ARGS();                 //!< aliases
                typedef intr_ptr<NodeKey,Node> Pointer;   //!< alias

                //==============================================================
                //
                // members
                //
                //==============================================================
                const SharedPoint point;     //!< the shared point
                const Vertex      celerity;  //!< local celerity
                const_type        speed;     //!< |celerity|
                const Vertex      uT;        //!< celerity/|celerity|
                const Vertex      uN;        //!< local normal vector
                const Vertex      Q;         //!< quadratic coefficient
                const Vertex      W;         //!< cubic coefficient
                const NodeKey     uuid;      //!< UUID

                //==============================================================
                //
                // methods
                //
                //==============================================================

                //! setup
                inline explicit Node( const SharedPoint &p ) throw() :
                point(p),
                celerity(),
                speed(0),
                uT(),
                uN(),
                Q(),
                W(),
                uuid(*p,*this)
                {
                }

                //! cleanup
                inline virtual ~Node() throw()
                {
                    setFixed();
                }

                //! for pointer/database
                inline const NodeKey & key() const throw() { return uuid; }

                //! update celerity from evaluation
                inline void setCelerity( const Vertex v ) throw()
                {
                    setFixed();
                    const VTX  &V = aliasing::cast<VTX,Vertex>(v);
                    const_type v2 = V.norm2();
                    if(v2>0)
                    {
                        aliasing::_(celerity) = v;
                        aliasing::_(speed)    = sqrt_of(v2);
                        aliasing::_(uT)       = celerity/speed;
                    }
                }

                //! set as a fixed point
                inline void setFixed() throw()
                {
                    bzset_(celerity);
                    bzset_(speed);
                    bzset_(uT);
                    bzset_(uN);
                    bzset_(Q);
                    bzset_(W);
                }

                //! local evaluation
                inline Vertex compute(const_type u) const
                {
                    return point->position + u*(celerity + u*(Q + u*W));
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Node);


            };

            //==================================================================
            //
            //! forwarding node types
            //
            //==================================================================
#define Y_EUCLIDEAN_NODE_ARGS()                \
typedef Node<T,POINT>              NodeType;   \
typedef typename NodeType::Pointer SharedNode
            

        }

    }

}

#endif

