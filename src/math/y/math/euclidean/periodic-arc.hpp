//! \file
#ifndef Y_EUCLIDEAN_PERIODIC_ARC_INCLUDED
#define Y_EUCLIDEAN_PERIODIC_ARC_INCLUDED 1

#include "y/math/euclidean/arc.hpp"
#include "y/ptr/auto.hpp"

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

                virtual bool check() const throw()
                {
                    switch(this->points.size)
                    {
                        case 0: break;
                        case 1: if(this->segments.size>0) return false; break;
                        default:
                            if(this->segments.size!=this->points.size) return false;

                    }
                    return true;
                }

                inline virtual void celerities() throw()
                {
                    NodeList &pts = (NodeList&)(this->points);
                    switch(pts.size)
                    {
                        case 0: break;
                        case 1: bzset_(pts.head->celerity); break;
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

            private:
                Y_DISABLE_COPY_AND_ASSIGN(PeriodicArc);
                inline virtual void add( const SharedPoint &sp )
                {
                    SegmList &seg = (SegmList&)(this->segments);
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


            };
        }

    }

}

#endif


