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
                const Vertex      Q;         //!< quadratic
                const Vertex      A;         //!< acceleration
                const Vertex      B;         //!< acceleration at next point
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
                Q(),
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
                    bzset_(Q);
                    bzset_(A);
                    bzset_(B);
                }

                //! copy is ADDR!=NULL
#define Y_EUCLIDEAN_XCOPY(ADDR,VALUE) do { if(ADDR) { *(ADDR) = (VALUE); } } while(false)
                
                //! zero if ADDR!=NULL
#define Y_EUCLIDEAN_XZERO(ADDR)       do { if(ADDR) { bzset(*(ADDR));    } } while(false)

                //! local evaluation
                inline void compute(const_type u, Vertex *P, Vertex *dP, Vertex *d2P) const throw()
                {
                    const_type uo2 = u/2;
                    const_type uo3 = u/3;
                    const_type omu = 1-u;
                    Y_EUCLIDEAN_XCOPY(P,   point->position + u*(V+uo2 * ( (1-uo3)*A + uo3 * B + uo3*(1-uo2) * Q) ));
                    Y_EUCLIDEAN_XCOPY(dP,  V+u*( (1-uo2) * A + uo2 * B + uo2*(1-(uo3+uo3) )*Q) );
                    Y_EUCLIDEAN_XCOPY(d2P, A*omu+u*B+(u*omu)*Q);
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

