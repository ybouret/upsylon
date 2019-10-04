//! \file
#ifndef Y_EUCLIDEAN_SEGMENT_INCLUDED
#define Y_EUCLIDEAN_SEGMENT_INCLUDED 1

#include "y/math/euclidean/point.hpp"
#include "y/ptr/intr.hpp"

namespace upsylon {

    namespace math {

        namespace Euclidean {

            typedef key_address<2> SegmentKey;

            template <typename T,template <class> class POINT>
            class Segment : public Object
            {
            public:
                Y_EUCLIDEAN_POINT_ARGS();
                Y_EUCLIDEAN_NODE_ARGS();
                typedef intr_ptr<SegmentKey,Segment> Pointer;

                const SharedNode tail;
                const SharedNode head;
                const SegmentKey uuid;

                inline Segment(const SharedNode &a, const SharedNode &b) throw() :
                tail(a), head(b), uuid(*tail,*head)
                {
                }

                inline ~Segment() throw()
                {

                }

                const SegmentKey & key() const throw() { return uuid; }



            private:
                Y_DISABLE_COPY_AND_ASSIGN(Segment);
            };

#define Y_EUCLIDEAN_SEGMENT_ARGS() \
typedef Segment<T,POINT>              SegmentType;\
typedef typename SegmentType::Pointer SharedSegment

        }

    }
}

#endif
