

//! \file
#ifndef Y_GEOMETRIC_ARC_PERIODIC_INCLUDED
#define Y_GEOMETRIC_ARC_PERIODIC_INCLUDED 1

#include "y/mkl/geometric/arc.hpp"
#include "y/mkl/kernel/cyclic.hpp"

namespace upsylon
{

    namespace mkl
    {

        namespace Geometric
        {

            //__________________________________________________________________
            //
            //! periodic arc
            //__________________________________________________________________
            template <typename T,template <class> class VTX>
            class PeriodicArc : public Arc<T,VTX>
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
                static const size_t Dimensions = ArcType::Dimensions;

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                inline explicit PeriodicArc() : ArcType() {}
                inline virtual ~PeriodicArc() throw() {}

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                
                void insert( const SharedPoint &point )
                {
                    Nodes           &nodes  = aliasing::_(this->nodes);
                    const size_t     count  = nodes.size();
                    const SharedNode node   = new NodeType(point);
                    const NodeType  *origin = (count>0) ? & *nodes.back() : NULL;
                    nodes << node;

                    Segments &segments = aliasing::_(this->segments);
                    switch(count)
                    {
                        case 0:
                            assert(1==nodes.size());
                            assert(0==segments.size());
                            break;

                        case 1:
                            assert(2==nodes.size());
                            assert(0==segments.size());
                            // create first two segments
                            try {
                                SegmentType seg( & *nodes.front(), & *nodes.back() );
                                segments << seg;
                                seg.reverse();
                                segments << seg;
                            }
                            catch(...) {
                                segments.free();
                                nodes.pop_back();
                                throw;
                            }
                            break;

                        default:
                            assert(nodes.size()>2);
                            assert(1+segments.size()==nodes.size());
                            // create new segment
                            try {
                                const NodeType *current = node.content();
                                {
                                    const SegmentType segment(origin,current);
                                    segments << segment;
                                }
                                const size_t ns = segments.size();
                                bswap(segments[ns-1],segments[ns]);
                                assert(segments.back().finish== nodes.front().content());
                                segments.back().resetOrigin(current);
                            }
                            catch(...) {
                                nodes.pop_back();
                                throw;
                            }

                            break;
                    }
                }

                void build()
                {
                    Nodes       &nodes = aliasing::_(this->nodes);
                    const size_t count = nodes.size();
                    try {
                        if(count<3)
                        {
                            for(size_t i=count;i>0;--i)
                            {
                                bzset(nodes[i]->A);
                            }
                        }
                        else
                        {
                            const type factor = 6;
                            matrix<type> rhs(Dimensions,count);
                            vector<type> cof(count,0);
                            cyclic<type> cyc(count);
                            cyc.set(1,4,1);

                            this->fill_bulk(rhs,1,factor,***nodes.back(),***nodes[1], ***nodes[2]);
                            for(size_t im=1,i=2,ip=3;i<count;++i,++im,++ip)
                            {
                                this->fill_bulk(rhs,i,6,***nodes[im],***nodes[i],***nodes[ip]);
                            }
                            this->fill_bulk(rhs,count,factor, ***nodes[count-1], ***nodes.back(), ***nodes.front());
                            std::cerr << "rhs=" << rhs << std::endl;

                            for(size_t row=1,dim=0;row<=Dimensions;++row,++dim)
                            {
                                cyc.solve(cof,rhs[row]);
                                for(size_t i=count;i>0;--i)
                                {
                                    vertex &A = nodes[i]->A;
                                    type   *p = (type*)&A;
                                    p[dim] = cof[i];
                                }
                            }
                            for(size_t i=1;i<=count;++i)
                            {
                                std::cerr << "A[" << i << "]=" << nodes[i]->A << std::endl;
                            }

                        }
                    }
                    catch(...)
                    {
                        for(size_t i=count;i>0;--i)
                        {
                            bzset(nodes[i]->A);
                        }
                        throw;
                    }
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(PeriodicArc);


            };

        }

    }

}

#endif
