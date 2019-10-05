//! \file

#ifndef Y_EUCLIDEAN_NODE_INCLUDED
#define Y_EUCLIDEAN_NODE_INCLUDED 1

#include "y/math/euclidean/basis.hpp"
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
                Y_EUCLIDEAN_POINT_ARGS();                      //!< aliases
                typedef intr_ptr<NodeKey,Node>      Pointer;   //!< alias
                typedef __Basis<T,POINT,Dimensions> Basis;     //!< basis type {t,n[,b]}


                //==============================================================
                //
                // members
                //
                //==============================================================
                const SharedPoint point;     //!< the shared point
                const Vertex      V;         //!< local celerity
                const_type        speed;     //!< |celerity|
                const Basis       basis;     //!< local Frenet basis
                const Vertex      A;         //!< quadratic coefficient
                const Vertex      B;         //!< cubic coefficient
                const NodeKey     uuid;      //!< UUID

                //==============================================================
                //
                // methods
                //
                //==============================================================

                //! setup
                inline explicit Node( const SharedPoint &p ) throw() :
                point(p),
                V(),
                speed(0),
                basis(),
                A(),
                B(),
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
                    aliasing::_(V) = v;
                    const_type  v2 = aliasing::cast<VTX,Vertex>(V).norm2();
                    if(v2>0)
                    {
                        aliasing::_(speed)    = sqrt_of(v2);
                        aliasing::_(basis.t)  = V/speed;
                    }
                }

                //! set as a fixed point
                inline void setFixed() throw()
                {
                    bzset_(V);
                    bzset_(speed);
                    aliasing::_(basis).zero();
                    bzset_(A);
                    bzset_(B);
                }

#define Y_EUCLIDEAN_XCOPY(ADDR,VALUE) do { if(ADDR) { *(ADDR) = (VALUE); } } while(false)
#define Y_EUCLIDEAN_XZERO(ADDR)       do { if(ADDR) { bzset(*(ADDR));    } } while(false)

                //! local evaluation
                inline void compute(const_type u, Vertex *P, Vertex *dP, Vertex *d2P) const throw()
                {
                    Y_EUCLIDEAN_XCOPY(P,  point->position + u*(V + u*(A + u*B)) );
                    Y_EUCLIDEAN_XCOPY(dP, V+2*u*A+3*u*u*B);
                    Y_EUCLIDEAN_XCOPY(d2P,2*A+6*u*B);
                }

                //! for 3D
                void finalize3D(const Vertex &N) throw()
                {
                    aliasing::_(basis).finalize(N);
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

