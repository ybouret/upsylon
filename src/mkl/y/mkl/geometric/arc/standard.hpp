
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
                typedef typename   ArcType::NodeType     NodeType;     //!< alias
                typedef typename   ArcType::SharedNode   SharedNode;   //!< alias
                typedef typename   ArcType::vertex       vertex;       //!< alias
                typedef typename   ArcType::Nodes        Nodes;        //!< alias
                typedef typename   ArcType::SegmentType  SegmentType;  //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                inline explicit StandardArc() : ArcType() {} //!< setup
                inline virtual ~StandardArc() throw() {}     //!< cleanup

               

                inline virtual type tauMax() const throw()
                {
                    return type(this->nodes.size());
                }

                vertex A(type tau) const throw()
                {
                    if(tau<=1)
                    {
                        return this->nodes.front()->A;
                    }
                    else
                    {
                        const_type top = tauMax();
                        if(tau>=top)
                        {
                            return this->nodes.back()->A;
                        }
                        else
                        {
                            assert(this->segments.size>0);
                            const SegmentType *segment = this->segments.head;
                            type im=1;
                            type ip=2;
                            while(!(im<=tau&&tau<=ip))
                            {
                                ++im;
                                ++ip;
                                segment = segment->next;
                            }
                            return segment->A(tau-im);
                        }
                    }
                }

                
               

            private:
                Y_DISABLE_COPY_AND_ASSIGN(StandardArc);
                
                //! insert and update segments
                inline virtual void insert_(const SharedPoint &point)
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
            };

        }

    }

}

#endif
