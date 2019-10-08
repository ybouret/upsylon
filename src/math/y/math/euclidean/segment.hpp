//! \file
#ifndef Y_EUCLIDEAN_SEGMENT_INCLUDED
#define Y_EUCLIDEAN_SEGMENT_INCLUDED 1

#include "y/math/euclidean/node.hpp"

namespace upsylon {

    namespace math {

        namespace Euclidean {


            typedef key_address<2> SegmentKey;

            template <typename T, template <class> class VTX>
            class Segment : public Object
            {
            public:
                Y_EUCLIDEAN_NODE_TYPES();
                typedef intr_ptr<SegmentKey,Segment> Pointer;

                const SharedNode tail;
                const SharedNode head;
                const SegmentKey uuid;

                inline virtual ~Segment() throw() {}
                inline explicit Segment(const SharedNode &a, const SharedNode &b) throw() :
                tail(a), head(b), uuid( *tail, *head )
                {
                }

                inline const SegmentKey & key() const throw() { return uuid; }

                void updateDelta(const ArcClass C) throw()
                {
                    NodeType &node = aliasing::_(*tail);
                    switch(C)
                    {
                        case Arc2: aliasing::_(node.dA) = head->A-tail->A;
                        case Arc1: aliasing::_(node.dV) = head->V-tail->V;
                        case Arc0: aliasing::_(node.dP) = head->P-tail->P;
                    }
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Segment);
            };

#define Y_EUCLIDEAN_SEGMENT_TYPES() \
Y_EUCLIDEAN_NODE_TYPES(); \
typedef Segment<T,VTX> SegmentType;\
typedef typename SegmentType::Pointer SharedSegment

        }

    }

}

#endif

