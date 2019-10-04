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
                Y_EUCLIDEAN_ARC_ARGS();

                inline virtual ~PeriodicArc() throw() {}
                inline explicit PeriodicArc() throw() {}


                inline virtual void ensure(const size_t numNodes)
                {
                    aliasing::_(this->nodes).ensure(numNodes);
                    aliasing::_(this->segments).ensure(numNodes);
                }


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
                            pts.tail->setCelerity(  -pts.head->celerity );
                            break;
                        default: assert(pts.size>=3);
                            this->celerityOf(pts.tail,pts.head,pts.head->next);
                            for(NodeType *curr=pts.head->next;curr!=pts.tail;curr=curr->next)
                            {
                                this->celerityOf(curr->prev, curr, curr->next);
                            }
                            this->celerityOf(pts.tail->prev,pts.tail,pts.head);
                            break;

                    }
                }

                inline virtual void compile()
                {
                    this->compileStd();
                    NodeList &pts = aliasing::_(this->points);
                    this->curveFor(pts.tail,pts.head);
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(PeriodicArc);
                inline virtual void add( const SharedPoint &sp )
                {
                    SegmList &seg = aliasing::_(this->segments);
                    switch(this->points.size)
                    {
                            //--------------------------------------------------
                            // first point
                        case 0: assert(0==this->segments.size); this->pushBack(sp); assert(1==this->points.size); break;
                            //--------------------------------------------------

                            //--------------------------------------------------
                            // first couple of points => two segments
                        case 1:
                            //--------------------------------------------------
                            this->pushBack(sp); assert(2==this->points.size);
                            try {
                                seg(*(this->points.head),*(this->points.tail)); // initial growing
                                seg(*(this->points.tail),*(this->points.head)); // initial closing
                            } catch(...) {
                                this->popBack();
                                seg.release();
                                throw;
                            }
                            break;

                            //--------------------------------------------------
                            // expansion
                            //--------------------------------------------------
                        default:assert(2<=this->points.size); assert(this->segments.size==this->points.size);
                            this->pushBack(sp);
                            try {
                                // create 2 new segments
                                auto_ptr<SegmentType> growing = new SegmentType(*(this->points.tail->prev),*(this->points.tail));
                                auto_ptr<SegmentType> closing = new SegmentType(*(this->points.tail),*(this->points.head));

                                // no-throw replacement
                                delete seg.pop_back(); // old closing
                                seg.push_back( growing.yield() );
                                seg.push_back( closing.yield() );
                            } catch(...) {
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


