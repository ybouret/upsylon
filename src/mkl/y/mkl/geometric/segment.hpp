
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

            template <typename T, template <class> class VTX>
            class Segment : public object
            {
            public:
                typedef core::list_of_cpp<Segment> List;
                typedef Node<T,VTX>                NodeType;
                typedef typename NodeType::Pointer SharedNode;

                inline virtual ~Segment() throw() {}
                inline explicit Segment(const NodeType *headNode,
                                        const NodeType *tailNode) throw() :
                next(0), prev(0),
                head( headNode  ),
                tail( tailNode  )
                {
                    assert(NULL!=tailNode);
                    assert(NULL!=headNode);
                }
                
                Segment        *next;
                Segment        *prev;
                const NodeType *head;
                const NodeType *tail;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Segment);
            };

        }

    }

}

#endif
