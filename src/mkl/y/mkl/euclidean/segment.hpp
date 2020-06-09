//! \file
#ifndef Y_MKL_EUCLIDEAN_SEGMENT_INCLUDED
#define Y_MKL_EUCLIDEAN_SEGMENT_INCLUDED 1

#include "y/mkl/euclidean/node.hpp"

namespace upsylon {

    namespace mkl {

        namespace Euclidean {

            //==================================================================
            //
            //! key for segments
            //
            //==================================================================
            typedef key_address<2> SegmentKey;

            //==================================================================
            //
            //! segment between tail and head points
            //
            //==================================================================
            template <typename T, template <class> class VTX>
            class Segment : public Object
            {
            public:
                Y_EUCLIDEAN_NODE_TYPES();                     //!< aliases
                typedef intr_ptr<SegmentKey,Segment> Pointer; //!< alias

                const SharedNode tail; //!< tail point
                const SharedNode head; //!< head point
                const SegmentKey uuid; //!< unique identifier

                //! cleanup
                inline virtual ~Segment() throw() {}

                //! setup
                inline explicit Segment(const SharedNode &a, const SharedNode &b) throw() :
                tail(a), head(b), uuid( *tail, *head )
                {
                }

                //! key for database
                inline const SegmentKey & key() const throw() { return uuid; }

                //! build local data for tail (origin) node
                void build(const ArcClass C) throw()
                {
                    const NodeType &node = aliasing::_(*tail);

                    switch(C)
                    {
                        case Arc2: aliasing::_(node.dA) = head->A-tail->A; /* FALLTHRU */
                        case Arc1: aliasing::_(node.dV) = head->V-tail->V; /* FALLTHRU */
                        case Arc0: aliasing::_(node.dP) = head->P-tail->P;
                    }

                    switch(C)
                    {
                        case Arc0: break;
                        case Arc1: aliasing::_(node.Q) = 6*node.dP - 3*(tail->V+head->V); break;
                        case Arc2:
                            aliasing::_(node.Q) = 60*node.dP - 6*(6*tail->V+4*head->V) - 2*(4*tail->A - head->A);
                            aliasing::_(node.W) = 60*(tail->V+head->V)-120*node.dP - 10*node.dA;
                            break;
                    }

                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Segment);
            };

            //! forward nested types
#define Y_EUCLIDEAN_SEGMENT_TYPES() \
Y_EUCLIDEAN_NODE_TYPES(); \
typedef Euclidean::Segment<T,VTX> SegmentType;\
typedef typename SegmentType::Pointer SharedSegment

        }

    }

}

#endif

