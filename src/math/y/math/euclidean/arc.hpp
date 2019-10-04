//! \file
#ifndef Y_EUCLIDEAN_ARC_INCLUDED
#define Y_EUCLIDEAN_ARC_INCLUDED 1

#include "y/math/euclidean/segment.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon {
    
    namespace math {
        
        namespace Euclidean {
            
            //! forwarding types
#define Y_EUCLIDEAN_ARC_ARGS() \
Y_EUCLIDEAN_POINT_ARGS();      \
Y_EUCLIDEAN_SEGMENT_ARGS();    \
Y_EUCLIDEAN_POINTNODE_ARGS();  \
typedef Segments<T,POINT> SegmList;\
typedef Arc<T,POINT>      ArcType;

            //==================================================================
            //
            //
            //! base class for Arc operations
            //
            //
            //==================================================================
            template <typename T, template <class> class POINT>
            class Arc : public Object
            {
            public:

                Y_EUCLIDEAN_ARC_ARGS(); //!< aliases
                
                
                inline virtual ~Arc() throw() {}
                
                inline Arc & operator<<( const SharedPoint &sp )
                {
                    add(sp);
                    return *this;
                }
                
                inline Arc & operator<<( const Vertex &v )
                {
                    const SharedPoint sp = new PointType(v);
                    return (*this) << sp;
                }
                
                const NodeList points;
                const SegmList segments;
                
                virtual bool check() const throw() = 0;
                virtual void celerities() throw() = 0;
                
            protected:
                inline explicit Arc() throw() {}
                
                
                inline void pushBack( const SharedPoint &sp )
                {
                    ((NodeList&)points).push_back( new NodeType(sp) );
                }
                
                inline void popBack() throw()
                {
                    assert(points.size>0);
                    delete ((NodeList&)points).pop_back();
                }
                
                inline void celerityOf(const NodeType *prev,
                                       NodeType       *curr,
                                       const NodeType *next) throw()
                {
                    static const_type half(0.5);
                    assert(prev); assert(curr); assert(next);
                    curr->celerity = half*((*next)->position - (*prev)->position);
                }
                
            private:
                Y_DISABLE_ASSIGN(Arc);
                virtual void add( const SharedPoint &sharedPoint ) = 0;
                
            };

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
                
                virtual bool check() const throw()
                {
                    switch(this->points.size)
                    {
                        case 0: break;
                        case 1: if(this->segments.size>0) return false; break;
                        default:
                            if(this->segments.size+1!=this->points.size) return false;
                            
                    }
                    return true;
                }
                
                inline virtual void celerities() throw()
                {
                    NodeList &pts = (NodeList&)(this->points);
                    switch(pts.size)
                    {
                        case 0: break;
                        case 1: bzset(pts.head->celerity); break;
                        case 2:
                            pts.head->celerity = (*pts.head)->position - (*pts.tail)->position;
                            pts.tail->celerity = pts.head->celerity;
                            break;
                        default: { assert(pts.size>=3);
                            static const_type half(0.5);
                            static const_type four(4);
                            static const_type three(3);
                            {
                                const Vertex &P0 = (**(pts.head)).position;
                                const Vertex &P1 = (**(pts.head->next)).position;
                                const Vertex &P2 = (**(pts.head->next->next)).position;
                                pts.head->celerity = half*( four * P1 - (P2+three*P0 ));
                                
                            }
                            
                            for(NodeType *curr=pts.head->next;curr!=pts.tail;curr=curr->next)
                            {
                                this->celerityOf(curr->prev, curr, curr->next);
                            }
                            
                            {
                                const Vertex &P0 = (**(pts.tail)).position;
                                const Vertex &P1 = (**(pts.tail->prev)).position;
                                const Vertex &P2 = (**(pts.tail->prev->prev)).position;
                                pts.tail->celerity = half*(  (P2+three*P0 ) - four * P1 );
                            }
                            

                        } break;
                            
                    }
                }
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(StandardArc);
                
                inline virtual void add( const SharedPoint &sp )
                {
                    SegmList &seg = (SegmList&)(this->segments);
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

            };


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
                        case 1: bzset(pts.head->celerity); break;
                        case 2:
                            pts.head->celerity = (*pts.head)->position - (*pts.tail)->position;
                            pts.tail->celerity = -pts.head->celerity;
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

