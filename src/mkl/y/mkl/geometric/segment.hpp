
//! \file
#ifndef Y_GEOMETRIC_SEGMENT_INCLUDED
#define Y_GEOMETRIC_SEGMENT_INCLUDED 1

#include "y/mkl/geometric/node.hpp"

namespace upsylon
{

    namespace mkl
    {

        namespace Geometric
        {

            //__________________________________________________________________
            //
            //! lightweight segment for connectivity
            //__________________________________________________________________
            template <typename T, template <class> class VTX>
            class Segment
            {
            public:
                //______________________________________________________________
                //
                // types and defintions
                //______________________________________________________________
                Y_DECL_ARGS(T,type);                           //!< aliases
                typedef Node<T,VTX>                NodeType;   //!< alias
                typedef VTX<T>                     vertex;     //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! cleanup
                inline   ~Segment() throw() {}

                //! setup head --> tail
                inline   Segment(const NodeType *orgNode,
                                 const NodeType *finNode) throw() :
                origin( orgNode  ),
                finish( finNode  ),
                deltaP( ***finish - ***origin )
                {

                }

                inline Segment(const Segment &segment) throw() :
                origin(segment.origin),
                finish(segment.finish),
                deltaP(segment.deltaP)
                {
                }

                //______________________________________________________________
                //
                // members
                //______________________________________________________________

                inline void resetOrigin(const NodeType *orgNode) throw()
                {
                    assert(orgNode);
                    aliasing::_(origin) = orgNode;
                    aliasing::_(deltaP) = ***finish - ***origin;
                }

                //! reverse order
                inline void reverse() throw()
                {
                    _cswap(origin,finish);
                    aliasing::_(deltaP) = -deltaP;
                }

                //! position
                inline vertex P(const type beta) const throw()
                {
                    static const type one = 1;
                    const vertex Pm = ***origin;
                    const vertex Am = origin->A;
                    const vertex Pp = ***finish;
                    const vertex Ap = finish->A;

                    const type  alpha = one-beta;
                    const type  a3ma  = alpha*alpha*alpha - alpha;
                    const type  b3mb  = beta*beta*beta    - beta;
                    return (alpha * Pm + beta * Pp) + (a3ma * Am + b3mb * Ap)/6;
                }

                //! speed
                inline vertex V(const type beta) const throw()
                {
                    static const type one = 1;
                    const vertex Am = origin->A;
                    const vertex Ap = finish->A;

                    const type  alpha = one-beta;
                    const type  b2    = 3*beta*beta - one;
                    const type  a2    = one  - 3*alpha*alpha;
                    return deltaP + (a2*Am+b2*Ap)/6;
                }


                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const NodeType * const origin;  //!< origin point
                const NodeType * const finish;  //!< finish point
                const vertex     deltaP;  //!< finish-origin;

            private:
                Y_DISABLE_ASSIGN(Segment);
            };

        }

    }

}

#endif
