
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
                deltaP(),
                deltaA()
                {

                }

                //! copy
                inline Segment(const Segment &segment) throw() :
                origin(segment.origin),
                finish(segment.finish),
                deltaP(segment.deltaP),
                deltaA(segment.deltaA)
                {
                }

                //______________________________________________________________
                //
                // members
                //______________________________________________________________

                //! reset origin node, mostly for periodic arcs
                inline void resetOrigin(const NodeType *orgNode) throw()
                {
                    assert(orgNode);
                    aliasing::_(origin) = orgNode;
                }

                //! reverse order
                inline void reverse() throw()
                {
                    _cswap(origin,finish);
                }

                //! pre-compile
                inline void compile() throw()
                {
                    aliasing::_(deltaP) = ***finish - ***origin;
                    aliasing::_(deltaA) = finish->A - origin->A;
                }

                //! position
                inline vertex P(const type tau) const throw()
                {
                    static const type one   = 1;
                    static const type six   = 6;

                    const vertex Pm = ***origin;
                    const vertex Am = origin->A;
                    const vertex Pp = ***finish;
                    const vertex Ap = finish->A;

                    const type  alpha     = one-tau;
                    const type  beta      = tau;
                    const type  six_gamma = (alpha*alpha-one)*alpha;
                    const type  six_delta = (beta*beta  -one)*beta;
                    return (alpha * Pm + beta * Pp) + (six_gamma * Am + six_delta * Ap)/six;
                }

                inline vertex V(const type tau) const throw()
                {
                    static const type one   = 1;
                    static const type two   = 2;
                    static const type three = 3;
                    static const type six   = 6;

                    const vertex Am = origin->A;
                    const vertex Ap = finish->A;

                    const type  six_gamma_p = (six-three*tau)*tau-two;
                    const type  six_delta_p = (three*tau*tau-one);

                    return deltaP + (six_gamma_p * Am + six_delta_p * Ap)/six;
                }

                inline vertex A(const type tau) const throw()
                {
                    static const type one   = 1;
                    
                    const vertex Am = origin->A;
                    const vertex Ap = finish->A;
                    return (one-tau)*Am + tau * Ap;
                }



                //! position
                inline vertex Q(const type tau) const throw()
                {
                    static const type one   = 1;
                    static const type half  = type(0.5);
                    static const type six   = 6;
                    static const type three = 3;

                    const vertex Pm = ***origin;
                    const vertex Am = origin->A;
                    const vertex Pp = ***finish;
                    const vertex Ap = finish->A;

                    const type  alpha     = one-tau;
                    const type  beta      = tau;
                    const type  six_gamma = (alpha*alpha-one)*alpha;
                    const type  six_delta = (beta*beta  -one)*beta;
                    const type  tmt       = alpha*beta;
                    const type  eta       = (half-tau)*tmt*(one+three*tmt)/30;
                    return (alpha * Pm + beta * Pp) + (six_gamma * Am + six_delta * Ap)/six + eta * deltaA;
                }

                inline vertex VQ(const type tau) const throw()
                {
                    static const type one   = 1;
                    static const type half  = type(0.5);
                    static const type two   = 2;
                    static const type three = 3;
                    static const type six   = 6;

                    const vertex Am = origin->A;
                    const vertex Ap = finish->A;

                    const type  six_gamma_p = (six-three*tau)*tau-two;
                    const type  six_delta_p = (three*tau*tau-one);
                    const type  tmt  = tau*(one-tau);
                    const type  eta_p = (one/30-tmt*tmt)*half;
                    return deltaP + (six_gamma_p * Am + six_delta_p * Ap)/six + eta_p * deltaA;
                }

                inline vertex AQ(const type tau) const throw()
                {
                    static const type one   = 1;

                    const vertex Am = origin->A;
                    const vertex Ap = finish->A;
                    const type   omt = one-tau;
                    const type   eta_s = tau * omt * (tau+tau-one);
                    return omt*Am + tau * Ap + eta_s * deltaA;
                }

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const NodeType * const origin;  //!< origin point
                const NodeType * const finish;  //!< finish point
                const vertex           deltaP;  //!< pre-compiled deltaP
                const vertex           deltaA;  //!< pre-compiled deltaA

            private:
                Y_DISABLE_ASSIGN(Segment);
            };

        }

    }

}

#endif
