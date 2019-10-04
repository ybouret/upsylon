//! \file
#ifndef Y_EUCLIDEAN_STANDARD_ARC_INCLUDED
#define Y_EUCLIDEAN_STANDARD_ARC_INCLUDED 1

#include "y/math/euclidean/arc.hpp"

namespace upsylon {

    namespace math {

        namespace Euclidean {

            //==================================================================
            //
            //
            //! Standard Arc
            //
            //
            //==================================================================
            template <typename T, template <class> class POINT>
            class StandardArc : public Arc<T,POINT>
            {
            public:
                Y_EUCLIDEAN_ARC_ARGS();

                inline virtual ~StandardArc() throw() {}
                inline explicit StandardArc() throw() : ArcType() {}

                inline virtual void ensure(const size_t numNodes)
                {
                    if(numNodes>0)
                    {
                        aliasing::_(this->nodes   ).ensure(numNodes  );
                        aliasing::_(this->segments).ensure(numNodes-1);
                    }
                }

                inline virtual void celerities() throw()
                {
                    Nodes       &nds = aliasing::_(this->nodes);
                    const size_t num = nds.size();
                    switch(num)
                    {
                        case 0: break;
                        case 1: nds.front()->setFixed(); break;
                        case 2: {
                            const Vertex delta = nds.back()->point->position - nds.front()->point->position;
                            nds.front()->setCelerity( delta );
                            nds.back()->setCelerity(  delta );
                        } break;
                        default: {
                            assert(nds.size()>=3);
                            static const_type half(0.5);
                            static const_type four(4);
                            static const_type three(3);
                            {
                                const Vertex &P0 = nds[1]->point->position;
                                const Vertex &P1 = nds[2]->point->position;
                                const Vertex &P2 = nds[3]->point->position;
                                nds.front()->setCelerity( half*( four * P1 - (P2+three*P0 )) );
                            }
                            for(size_t i=num-1;i>1;--i)
                            {
                                nds[i]->setCelerity(nds[i+1]->point->position-nds[i-1]->point->position);
                            }
                            {
                                const Vertex &P0 = nds[num-0]->point->position;
                                const Vertex &P1 = nds[num-1]->point->position;
                                const Vertex &P2 = nds[num-2]->point->position;
                                nds.back()->setCelerity( half*(  (P2+three*P0 ) - four * P1 ) );
                            }
                        } break;
                    }
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(StandardArc);
                virtual void add( const SharedPoint &p )
                {
                    switch( this->nodes.size() )
                    {
                        case 0: this->pushBack(p); break;
                        default:
                            this->pushBack(p);
                            try {
                                this->pushGrowing();
                            } catch(...) {
                                this->popBack();
                            } break;
                    }
#if !defined(NDEBUG)
                    if( this->nodes.size() > 0 )
                    {
                        assert(this->nodes.size()-1 == this->segments.size());
                        for(size_t i=1;i<this->segments.size();++i)
                        {
                            const SegmentType &s = *(this->segments[i]);
                            assert(s.tail->uuid==this->nodes[i]->uuid);
                            assert(s.head->uuid==this->nodes[i+1]->uuid);

                        }
                    }
#endif
                }


#if 0 

                inline virtual void celerities() throw()
                {
                    NodeList &pts = aliasing::_(this->points);
                    switch(pts.size)
                    {
                        case 0: break;
                        case 1: pts.head->setFixed(); break;
                        case 2:
                            pts.head->setCelerity( (*pts.head)->position - (*pts.tail)->position );
                            pts.tail->setCelerity( pts.head->celerity );
                            break;
                        default: { assert(pts.size>=3);
                            static const_type half(0.5);
                            static const_type four(4);
                            static const_type three(3);
                            {
                                const Vertex &P0 = (**(pts.head)).position;
                                const Vertex &P1 = (**(pts.head->next)).position;
                                const Vertex &P2 = (**(pts.head->next->next)).position;
                                pts.head->setCelerity( half*( four * P1 - (P2+three*P0 )) );

                            }

                            for(NodeType *curr=pts.head->next;curr!=pts.tail;curr=curr->next)
                            {
                                this->celerityOf(curr->prev, curr, curr->next);
                            }

                            {
                                const Vertex &P0 = (**(pts.tail)).position;
                                const Vertex &P1 = (**(pts.tail->prev)).position;
                                const Vertex &P2 = (**(pts.tail->prev->prev)).position;
                                pts.tail->setCelerity( half*(  (P2+three*P0 ) - four * P1 ) );
                            }


                        } break;

                    }
                }

                inline void compile()
                {
                    this->compileStd();
                }

                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(StandardArc);

                inline virtual void add( const SharedPoint &sp )
                {
                    SegmList &seg = aliasing::_(this->segments);
                    if(this->points.size<=0)
                    {
                        assert(this->segments.size<=0);
                        this->pushBack(sp);
                        return;
                    }
                    else
                    {
                        assert(this->points.size>=1); this->pushBack(sp); assert(this->points.size>=2);
                        try
                        {
                            const NodeType *tail = this->points.tail; assert(tail); assert(tail->prev);
                            seg(*(tail->prev),*tail);
                        }
                        catch(...)
                        {
                            this->popBack();
                            throw;
                        }
                    }
                }
#endif
            };

        }

    }

}

#endif


