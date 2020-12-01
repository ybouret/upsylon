
//! \file
#ifndef Y_GEOMETRIC_ARC_STANDARD_INCLUDED
#define Y_GEOMETRIC_ARC_STANDARD_INCLUDED 1

#include "y/mkl/geometric/arc.hpp"

namespace upsylon
{

    namespace mkl
    {

        namespace Geometric
        {

            template <typename T,template <class> class VTX>
            class StandardArc : public Arc<T,VTX>
            {
            public:
                typedef Arc<T,VTX> ArcType;
                typedef typename   ArcType::SharedPoint  SharedPoint;
                typedef typename   ArcType::NodeType     NodeType;
                typedef typename   ArcType::SharedNode   SharedNode;
                typedef typename   ArcType::vertex       vertex;
                typedef typename   ArcType::Nodes        Nodes;
                typedef typename   ArcType::SegmentType  SegmentType;

                inline explicit StandardArc() : ArcType() {}
                inline virtual ~StandardArc() throw() {}

                inline void insert(const SharedPoint &point)
                {
                    // check back node to get future head of segment
                    Nodes           &nodes = aliasing::_(this->nodes);
                    const size_t     count = nodes.size();
                    const NodeType  *head  = count ? nodes.back().content() : NULL;

                    // create new node got get future tail of segment
                    NodeType        *tail  = new NodeType(point);
                    const SharedNode node  = tail;

                    // insert node
                    if(!nodes.insert(node)) this->nodeInsertFailure( typeid(vertex) );

                    // create segment
                    if(head)
                    {
                        assert(this->nodes.size()>=2);
                        try
                        {
                            aliasing::_(this->segments).push_back( new SegmentType(head,tail) );
                        }
                        catch(...)
                        {
                            (void) nodes.remove(node->uuid);
                            throw;
                        }
                    }
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(StandardArc);
            };

        }

    }

}

#endif
