//! \file
#ifndef Y_EUCLIDEAN_ARC_INCLUDED
#define Y_EUCLIDEAN_ARC_INCLUDED 1

#include "y/math/euclidean/segment.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon {

    namespace math {

        namespace Euclidean {
            

#define Y_EUCLIDEAN_ARC_ARGS() \
Y_EUCLIDEAN_POINT_ARGS();      \
Y_EUCLIDEAN_SEGMENT_ARGS();    \
Y_EUCLIDEAN_POINTNODE_ARGS();  \
typedef Segments<T,POINT> SegmList;\
typedef Arc<T,POINT>      ArcType;

            template <typename T, template <class> class POINT>
            class Arc : public Object
            {
            public:
                Y_EUCLIDEAN_ARC_ARGS();


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

            private:
                Y_DISABLE_ASSIGN(Arc);
                virtual void add( const SharedPoint &sharedPoint ) = 0;

            };

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

