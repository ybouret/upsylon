
//! \file
#ifndef Y_GEOMETRIC_ARC_INCLUDED
#define Y_GEOMETRIC_ARC_INCLUDED 1

#include "y/mkl/geometric/segment.hpp"
#include "y/associative/hash/set.hpp"
#include "y/memory/buffers.hpp"
#include <typeinfo>

namespace upsylon
{

    namespace mkl
    {

        namespace Geometric
        {

            //__________________________________________________________________
            //
            //! common stuff for an arc
            //__________________________________________________________________
            class Arc_ : public Object
            {
            public:
                virtual ~Arc_() throw(); //!< cleanup

            protected:
                explicit Arc_() throw(); //!< setup
                
                //! throw exception, shouldn't happen
                void   nodeInsertFailure(const std::type_info &tid) const;



            private:
                Y_DISABLE_COPY_AND_ASSIGN(Arc_);
            };

            //__________________________________________________________________
            //
            //! base class for an arc
            //__________________________________________________________________
            template <typename T, template <class> class VTX>
            class Arc : public Arc_
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                Y_DECL_ARGS(T,type);                                     //!< aliases
                typedef Point<T,VTX>                     PointType;      //!< alias
                typedef typename PointType::Pointer      SharedPoint;    //!< alias
                typedef typename PointType::vertex       vertex;         //!< alias
                typedef Node<T,VTX>                      NodeType;       //!< alias
                typedef typename NodeType::Pointer       SharedNode;     //!< alias
                typedef hash_set<NodeKey,SharedNode>     Nodes;          //!< alias
                typedef Segment<T,VTX>                   SegmentType;    //!< alias
                typedef typename SegmentType::List       Segments;       //!< alias
                typedef SegmentType                     *SegmentPointer; //!< alias
                typedef memory::cppblock<SegmentPointer> LinearSegments; //!< alias

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                //! cleanup
                inline virtual ~Arc() throw() {}

                //! insert new point, update segments

                inline void insert(const SharedPoint &point)
                {
                    insert_(point);
                    aliasing::_(compiled) = false;
                }

                //! tau in [1:tauMax]
                virtual type tauMax() const throw() = 0;




                //! update bulk
                void bulk() throw()
                {
                    static const type half = type(0.5);
                    Nodes &nodes = aliasing::_(this->nodes);
                    size_t count = nodes.size();
                    if(count>2)
                    {
                        typename Nodes::iterator prev = nodes.begin();
                        typename Nodes::iterator curr = prev; ++curr;
                        typename Nodes::iterator next = curr; ++next;
                        for(count-=2;count>0;--count)
                        {
                            const NodeType &prevNode = **prev; const vertex &pm = **prevNode;
                            NodeType       &currNode = **curr; const vertex &p0 = **currNode;
                            const NodeType &nextNode = **next; const vertex &pp = **nextNode;
                            vertex &v = aliasing::_(currNode.V);
                            vertex &a = aliasing::_(currNode.A);
                            v=half*(pp-pm);
                            a=( (pp-p0) + (pm-p0) );

                            prev=curr;
                            curr=next;
                            ++next;
                        }
                    }
                }

                //! build segment data
                void build() throw()
                {
                    for(SegmentType *segment = aliasing::_(segments).head;segment;segment=segment->next)
                    {
                        segment->build();
                    }

                }

                inline void compile()
                {
                    bulk();
                    build();
                    aliasing::_(compiled) = true;
                }

                


                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const Nodes    nodes;    //!< table/list of nodes
                const Segments segments; //!< list of segments
                const bool     compiled; //!< flag

            protected:
                //! setup empty
                explicit Arc() : nodes(), segments(), compiled(false)
                {
                }
                

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Arc);
                virtual void insert_(const SharedPoint &point) = 0;
            };

        }

    }

}

#endif
