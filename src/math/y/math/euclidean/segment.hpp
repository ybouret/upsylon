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
                Y_EUCLIDEAN_POINT_TYPES();
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


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Segment);
            };

        }

    }

}

#endif

