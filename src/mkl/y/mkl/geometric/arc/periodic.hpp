

//! \file
#ifndef Y_GEOMETRIC_ARC_PERIODIC_INCLUDED
#define Y_GEOMETRIC_ARC_PERIODIC_INCLUDED 1

#include "y/mkl/geometric/arc.hpp"

namespace upsylon
{

    namespace mkl
    {

        namespace Geometric
        {

            template <typename T,template <class> class VTX>
            class PeriodicArc : public Arc<T,VTX>
            {
            public:
                typedef Arc<T,VTX> ArcType;
                typedef typename   ArcType::SharedPoint  SharedPoint;
                typedef typename   ArcType::NodeType     NodeType;
                typedef typename   ArcType::SharedNode   SharedNode;
                typedef typename   ArcType::vertex       vertex;
                typedef typename   ArcType::Nodes        Nodes;
                typedef typename   ArcType::SegmentType  SegmentType;


                inline explicit PeriodicArc() : ArcType() {}
                inline virtual ~PeriodicArc() throw() {}

                inline void insert(const SharedPoint &point)
                {
                    // check back node to get future head of segment
                    Nodes           &nodes = aliasing::_(this->nodes);
                    const size_t     count = nodes.size();
                    const NodeType  *head  = count ? nodes.back().content() : NULL;

                    // create new node
                    NodeType        *curr  = new NodeType(point);
                    const SharedNode node  = curr;

                    // insert node
                    if(!nodes.insert(node)) this->nodeInsertFailure( typeid(vertex) );

                    // create segment(s)
                    typename ArcType::Segments &segments = aliasing::_(this->segments);
                    switch(count)
                    {
                        case 0: assert(1==nodes.size()); assert(0==this->segments.size); break;
                        case 1: assert(2==nodes.size()); assert(0==this->segments.size);
                            try {
                                segments.push_back( new SegmentType(head,curr) );
                                segments.push_back( new SegmentType(curr,head) );
                            }
                            catch(...) {
                                (void) nodes.remove(node->uuid);
                                segments.release();
                                throw;
                            }
                            break;
                    }

                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(PeriodicArc);
            };

        }

    }

}

#endif
