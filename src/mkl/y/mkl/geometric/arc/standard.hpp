
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

                //! default lower boundary
                virtual void update_lower() throw()
                {
                    Nodes &nodes = aliasing::_(this->nodes);
                    switch(nodes.size())
                    {
                        case 0:
                        case 1:
                            break;
                          
                        case 2: {
                            NodeType       &origin = *nodes.front();
                            const NodeType &finish = *nodes.back();
                            _bzset(origin.A);
                            aliasing::_(origin.V) = **finish - **origin;
                         }  break;
                            
                        default: {
                            static const_type half  = type(0.5);
                            static const_type three = type(3);

                            typename Nodes::iterator it = nodes.begin();
                            NodeType                &n1 = **it;
                            const vertex             p2 = ****(++it);
                            const vertex             p3 = ****(++it);
                            const vertex             p1 = **n1;
                            aliasing::_(n1.V) = half*( (p2-p3) + three * (p2-p1) );
                            aliasing::_(n1.A) = (p1-p2) + (p3-p2);
                        } break;
                    }
                }
                
                //! default upper boundary
                virtual void update_upper() throw()
                {
                    Nodes &nodes = aliasing::_(this->nodes);
                    switch(nodes.size())
                    {
                        case 0:
                        case 1:
                            break;
                            
                        case 2: {
                            const NodeType &origin = *nodes.front();
                            NodeType       &finish = *nodes.back();
                            _bzset(origin.A);
                            aliasing::_(origin.V) = **finish - **origin;
                        }  break;
                            
                        default: {
                            static const_type half  = type(0.5);
                            static const_type three = type(3);
                            
                            typename Nodes::reverse_iterator it = nodes.rbegin();
                            NodeType                &n1 = **it;
                            const vertex             p2 = ****(++it);
                            const vertex             p3 = ****(++it);
                            const vertex             p1 = **n1;
                            aliasing::_(n1.V) = half*( (p3-p2) + three * (p1-p2) );
                            aliasing::_(n1.A) = (p1-p2) + (p3-p2);
                        } break;
                    }
                }
                
                
                inline virtual type tauMax() const throw()
                {
                    return type(this->nodes.size());
                }

                inline vertex A(type tau) const throw()
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
                            tau-=im;
                            return segment->A(tau);
                        }
                    }
                }

                
               

            private:
                Y_DISABLE_COPY_AND_ASSIGN(StandardArc);
                
                //! insert and update segments
                inline virtual void insert_(const SharedPoint &point)
                {
                    // check back node to get future origin of segment
                    Nodes           &nodes  = aliasing::_(this->nodes);
                    const size_t     count  = nodes.size();
                    const NodeType  *origin = count ? nodes.back().content() : NULL;

                    // create new node got get future finish of segment
                    NodeType        *finish = new NodeType(point);
                    const SharedNode node   = finish;

                    // insert node
                    if(!nodes.insert(node)) this->nodeInsertFailure( typeid(vertex) );

                    // create segment
                    if(origin)
                    {
                        assert(this->nodes.size()>=2);
                        try
                        {
                            aliasing::_(this->segments).push_back( new SegmentType(origin,finish) );
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
