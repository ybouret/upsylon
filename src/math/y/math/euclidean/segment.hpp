//! \file
#ifndef Y_EUCLIDEAN_SEGMENT_INCLUDED
#define Y_EUCLIDEAN_SEGMENT_INCLUDED 1

#include "y/math/euclidean/point.hpp"

namespace upsylon {

    namespace math {

        namespace Euclidean {

            //! pair of point
            template <typename T, template <class> class POINT>
            class Pair : public Object
            {
            public:
                Y_EUCLIDEAN_ARGS();
                typedef arc_ptr<Pair> Pointer;

                const SharedPoint tail;
                const SharedPoint head;
                const_type        norm2;
                const_type        length;

                inline virtual ~Pair() throw() {}
                inline explicit Pair(const SharedPoint &a, const SharedPoint &b) throw() : tail(a), head(b), norm2(0), length(0) {}

                inline void update() throw()
                {
                    const VTX &A = (VTX &)(*tail);
                    const VTX &B = (VTX &)(*head);
                    const VTX  delta(A,B);
                    (mutable_type &)norm2  = delta.norm2();
                    (mutable_type &)length = math::sqrt_of(norm2);
                }

                static inline bool AreTheSame(const Pair &p, const Pair &q) throw()
                {
                    return (p.tail==q.tail) && (p.head==q.head);
                }

                static inline bool AreOpposite(const Pair &p, const Pair &q) throw()
                {
                    return (p.tail==q.head) && (p.head==q.tail);
                }

                friend inline bool  operator==( const Pair &lhs, const Pair &rhs ) throw()
                {
                    return Pair::AreTheSame(lhs,rhs) || Pair::AreOpposite(lhs,rhs);
                }

                friend inline bool  operator!=( const Pair &lhs, const Pair &rhs ) throw()
                {
                    return !Pair::AreTheSame(lhs,rhs) && !Pair::AreOpposite(lhs,rhs);
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Pair);
            };

            template <typename T, template <class> class POINT>
            class Segment : public Pair<T,POINT>::Pointer, public core::inode< Segment<T,POINT> >
            {
            public:
                Y_EUCLIDEAN_ARGS();
                typedef core::inode<Segment>       iNode;
                typedef Pair<T,POINT>              PairType;
                typedef typename PairType::Pointer SharedPair;
                typedef core::list_of_cpp<Segment> List;

                inline virtual ~Segment() throw() {}
                inline explicit Segment( const SharedPair &sharedPair ) throw() : SharedPair(sharedPair), iNode() {}
                inline explicit Segment( const SharedPoint &a, const SharedPoint &b) : SharedPair( new PairType(a,b) ), iNode() {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Segment);
            };

            //!
            template <typename T, template <class> class POINT>
            class Segments : public Segment<T,POINT>::List
            {
            public:
                Y_EUCLIDEAN_ARGS();
                typedef Segment<T,POINT>                  SegmentType;
                typedef typename SegmentType::SharedPair  SharedPair;
                typedef typename SegmentType::List        SegmentList;

                inline explicit Segments() throw() : SegmentList() {}
                inline virtual ~Segments() throw() {}
                inline Segments(const Segments &_) : SegmentList(_) {}

                inline Segments & operator<<( const SharedPair &sharedPair)
                {
                    this->push_back( new SegmentType(sharedPair) );
                    return *this;
                }

                inline Segments & operator()(const SharedPoint &a, const SharedPoint &b)
                {
                    this->push_back( new SegmentType(a,b) );
                    return *this;
                }

                //! update all
                void update() throw()
                {
                    for( SegmentType *s = this->head; s; s=s->next )
                    {
                        (*s)->update();
                    }
                }

            private:
                Y_DISABLE_ASSIGN(Segments);
            };


        }

    }
}

#endif
