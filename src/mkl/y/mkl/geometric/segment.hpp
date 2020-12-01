
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
            class Segment : public object
            {
            public:
                //______________________________________________________________
                //
                // types and defintions
                //______________________________________________________________
                Y_DECL_ARGS(T,type);                           //!< aliases
                typedef core::list_of_cpp<Segment> List;       //!< alias
                typedef Node<T,VTX>                NodeType;   //!< alias
                typedef typename NodeType::Pointer SharedNode; //!< alias
                typedef VTX<T>                     vertex;     //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! cleanup
                inline virtual ~Segment() throw() {}

                //! setup head --> tail 
                inline explicit Segment(const NodeType *headNode,
                                        const NodeType *tailNode) throw() :
                next(0), prev(0),
                head( headNode  ),
                tail( tailNode  ),
                alpha(),
                beta()
                {
                    assert(NULL!=tailNode);
                    assert(NULL!=headNode);
                }

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                inline void build() throw()
                {
                    static const_type _60 = type(60);
                    static const_type _32 = type(32);
                    static const_type _28 = type(28);
                    static const_type _6  = type(6);
                    static const_type _4  = type(4);
                    assert(head); assert(tail);
                    const vertex Pm = ***head;
                    const vertex Pp = ***tail;
                    const vertex Vm = head->V;
                    const vertex Vp = tail->V;
                    const vertex Am = head->A;
                    const vertex Ap = tail->A;

                    const vertex _60_dP = _60 * (Pp-Pm);
                    alpha = (_32*Vp+_28*Vm) - (_6*Ap-_4*Am) - _60_dP;
                    beta  = _60_dP - (_28*Vp+_32*Vm) + (_4*Ap-_6*Am);
                }

                inline vertex A(const_type tau) const throw()
                {
                    static const_type one = type(1);
                    const_type        omt = one-tau;
                    assert(head); assert(tail);
                    const vertex Am = head->A;
                    const vertex Ap = tail->A;
                    return omt*Am + tau*Ap;
                }

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                Segment        *next;  //!< for list
                Segment        *prev;  //!< for list
                const NodeType *head;  //!< head point
                const NodeType *tail;  //!< tail point
                vertex          alpha; //!< coefficient
                vertex          beta;  //!< coefficient



            private:
                Y_DISABLE_COPY_AND_ASSIGN(Segment);
            };

        }

    }

}

#endif
