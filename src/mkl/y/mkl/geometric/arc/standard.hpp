
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
            //__________________________________________________________________
            //
            //! standard arc
            //__________________________________________________________________
            template <typename T,template <class> class VTX>
            class StandardArc : public Arc<T,VTX>
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                Y_DECL_ARGS(T,type);                                   //!< aliases
                typedef Arc<T,VTX> ArcType;                            //!< alias
                typedef typename   ArcType::SharedPoint  SharedPoint;  //!< alias
                typedef typename   ArcType::vertex       vertex;       //!< alias
                typedef typename   ArcType::NodeType     NodeType;     //!< alias
                typedef typename   ArcType::SharedNode   SharedNode;   //!< alias
                typedef typename   ArcType::Nodes        Nodes;        //!< alias
                typedef typename   ArcType::SegmentType  SegmentType;  //!< alias
                typedef typename   ArcType::Segments     Segments;     //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                inline explicit StandardArc() throw() : ArcType() {} //!< setup
                inline virtual ~StandardArc() throw()             {} //!< cleanup

                
                void insert( const SharedPoint &point )
                {
                    Nodes           &nodes  = aliasing::_(this->nodes);
                    const size_t     count  = nodes.size();
                    const NodeType  *origin = (count>0) ? & *nodes.back() : NULL;
                    const SharedNode node   = new NodeType(point);
                    nodes << node;
                    if(origin)
                    {
                        try
                        {
                            const NodeType   *finish = node.content();
                            const SegmentType segment(origin,finish);
                            aliasing::_(this->segments) << segment;
                            assert(this->segments.size()+1==this->nodes.size());
                        }
                        catch(...)
                        {
                            nodes.pop_back();
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
