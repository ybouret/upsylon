//! \file
#ifndef Y_EUCLIDEAN_SEGMENT_INCLUDED
#define Y_EUCLIDEAN_SEGMENT_INCLUDED 1

#include "y/math/euclidean/node.hpp"

namespace upsylon {

    namespace math {

        namespace Euclidean {

            //==================================================================
            //
            //! key for segments database
            //
            //==================================================================
            typedef key_address<2> SegmentKey;


            //==================================================================
            //
            //
            //! a segment between two nodes
            //
            //
            //==================================================================
            template <typename T,template <class> class POINT>
            class Segment : public Object
            {
            public:
                //==============================================================
                //
                // types and definition
                //
                //==============================================================
                Y_EUCLIDEAN_POINT_ARGS();                     //!< aliases
                Y_EUCLIDEAN_NODE_ARGS();                      //!< aliases
                typedef intr_ptr<SegmentKey,Segment> Pointer; //!< alias

                //==============================================================
                //
                // members
                //
                //==============================================================
                const SharedNode tail; //!< tail node
                const SharedNode head; //!< head node
                const SegmentKey uuid; //!< unique key for tail,head

                //! setup
                inline Segment(const SharedNode &a, const SharedNode &b) throw() :
                tail(a), head(b), uuid(*tail,*head)
                {
                }

                //! cleanup
                inline ~Segment() throw()
                {

                }

                //! for pointer/database
                const SegmentKey & key() const throw() { return uuid; }



            private:
                Y_DISABLE_COPY_AND_ASSIGN(Segment);
            };

            //==================================================================
            //
            //! forwarding segment types
            //
            //==================================================================
#define Y_EUCLIDEAN_SEGMENT_ARGS()                  \
typedef Segment<T,POINT>              SegmentType;  \
typedef typename SegmentType::Pointer SharedSegment

        }

    }
}

#endif
