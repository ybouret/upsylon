//! \file
#ifndef Y_EUCLIDEAN_PERIODIC_ARC_INCLUDED
#define Y_EUCLIDEAN_PERIODIC_ARC_INCLUDED 1

#include "y/math/euclidean/arc.hpp"

namespace upsylon {

    namespace math {

        namespace Euclidean {

            //==================================================================
            //
            //
            //! Periodic Arc
            //
            //
            //==================================================================
            template <typename T, template <class> class POINT>
            class PeriodicArc : public Arc<T,POINT>
            {
            public:
                //==============================================================
                //
                // types and definition
                //
                //==============================================================
                Y_EUCLIDEAN_ARC_ARGS(); //!< aliases

                //==============================================================
                //
                // methods
                //
                //==============================================================

                //! cleanup
                inline virtual ~PeriodicArc() throw() {}

                //! setup
                inline explicit PeriodicArc() throw() : ArcType()  {}

                //! acquire some memory
                inline virtual void ensure(const size_t numNodes)
                {
                    aliasing::_(this->nodes).ensure(numNodes);
                    aliasing::_(this->segments).ensure(numNodes);
                }


                //! compute local periodic metrics
                inline virtual void metrics() throw()
                {

                    Nodes       &nds = aliasing::_(this->nodes);
                    const size_t num = nds.size();
                    switch(num)
                    {
                        case 0: break;
                        case 1: nds.front()->setFixed(); break;
                        case 2:{
                            const Vertex delta = nds.back()->point->position - nds.front()->point->position;
                            nds.front()->setCelerity( delta );
                            nds.back()->setCelerity( -delta );
                        } break;
                        default: {
                            static const_type half(0.5);

                            nds[1]->setCelerity(half*(nds[2]->point->position-nds[num]->point->position) );
                            for(size_t i=num-1;i>1;--i)
                            {
                                nds[i]->setCelerity( half*(nds[i+1]->point->position-nds[i-1]->point->position) );
                            }
                            nds[num]->setCelerity(half*(nds[1]->point->position-nds[num-1]->point->position) );
                            
                        } break;
                    }
                    
                    // compute normals
                    this->computeNormals2D(int2type<Dimensions>());
                    if(num>=3)
                    {
                        this->computeNormals3D(int2type<Dimensions>());
                    }

                }

                inline virtual void compute( mutable_type u, Vertex *P, Vertex *dP, Vertex *d2P ) const throw()
                {
                    const size_t num = this->nodes.size();
                    switch(num)
                    {
                        case 0:
                            Y_EUCLIDEAN_XZERO(P);
                            Y_EUCLIDEAN_XZERO(dP);
                            Y_EUCLIDEAN_XZERO(d2P);
                            break;

                        case 1:
                            Y_EUCLIDEAN_XCOPY(P,this->nodes.front()->point->position);
                            Y_EUCLIDEAN_XZERO(dP);
                            Y_EUCLIDEAN_XZERO(d2P);
                            break;

                        default: {
                            mutable_type i = floor_of(u);
                            u-=i;
                            while(i<1)   i+=num;
                            while(i>num) i-=num;
                            const NodeType &node = *(this->nodes[clamp<size_t>(1,i,num)]);
                            return node.compute(u,P,dP,d2P);
                        } break;
                    }
                }
                

#if 0
                //! any u, periodically set in [1:n]
                inline virtual Vertex operator()( mutable_type u ) const throw()
                {
                    const size_t num = this->nodes.size();
                    switch(num)
                    {
                        case 0: return Vertex();
                        case 1: return this->nodes.front()->point->position;
                        default:
                            break;
                    }
                    type i = floor_of(u);
                    u-=i;
                    while(i<1)   i+=num;
                    while(i>num) i-=num;
                    return this->nodes[clamp<size_t>(1,i,num)]->compute(u,NULL);
                }
#endif


            private:
                Y_DISABLE_COPY_AND_ASSIGN(PeriodicArc);

                virtual void add( const SharedPoint &p )
                {
                    Segments &seg = aliasing::_(this->segments);
                    switch(this->nodes.size())
                    {
                        case 0:
                            assert(0==this->segments.size());
                            this->pushBack(p); break;

                        case 1:
                            assert(0==this->segments.size());
                            this->pushBack(p);
                            try {
                                this->pushGrowing();
                                this->pushClosing();
                            } catch(...) {
                                seg.free();
                                this->popBack();
                                throw;
                            } break;


                        default: {
                            this->pushBack(p);
                            const size_t ns = seg.size();
                            {
                                size_t count = 0;
                                try {
                                    this->pushGrowing(); ++count;
                                    this->pushClosing(); ++count;
                                } catch(...) {
                                    this->popSegments(count);
                                    this->popBack();
                                    throw;
                                }
                            }
                            assert(2+ns==seg.size());
                            seg.remove_at(ns);
                            assert(1+ns==seg.size());
                        } break;
                    }

#if !defined(NDEBUG)
                    if( this->nodes.size() > 1 )
                    {
                        assert(this->nodes.size()==this->segments.size());
                        for(size_t i=1;i<this->segments.size();++i)
                        {
                            const SegmentType &s = *(this->segments[i]);
                            assert(s.tail->uuid==this->nodes[i]->uuid);
                            assert(s.head->uuid==this->nodes[i+1]->uuid);
                        }
                        {
                            const SegmentType &s = *(this->segments.back());
                            assert(s.tail->uuid==this->nodes.back()->uuid);
                            assert(s.head->uuid==this->nodes.front()->uuid);
                        }
                    }
#endif
                }
                
                inline void computeNormals3D( int2type<2> ) throw()
                {
                }
                
                inline void computeNormals3D( int2type<3> ) throw()
                {
                    static const float half(0.5);
                    
                    Nodes       &nds = aliasing::_(this->nodes);
                    const size_t num = nds.size(); assert(num>=3);
                    const size_t nm1 = num-1;

                    {
                        const Vertex N = half*(nds[2]->V-nds[num]->V);
                        nds[1]->finalize3D(N);
                    }
                    for(size_t i=nm1;i>1;--i)
                    {
                        const Vertex N = half*(nds[i+1]->V-nds[i-1]->V);
                        nds[i]->finalize3D(N);
                    }
                    {
                        const Vertex N = half*(nds[1]->V-nds[nm1]->V);
                        nds[num]->finalize3D(N);
                    }
                }


            };
        }

    }

}

#endif


