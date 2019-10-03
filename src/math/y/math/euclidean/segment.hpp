//! \file
#ifndef Y_EUCLIDEAN_SEGMENT_INCLUDED
#define Y_EUCLIDEAN_SEGMENT_INCLUDED 1

#include "y/math/euclidean/point.hpp"
#include "y/container/key-address.hpp"
#include "y/ptr/intr.hpp"

namespace upsylon {

    namespace math {

        namespace Euclidean {

            //==================================================================
            //
            //
            //! using ordered addresses to make a key
            //
            //
            //==================================================================
            typedef key_address<2> PairKey;

            //==================================================================
            //
            //
            //! pair of points with data and unique key
            //
            //
            //==================================================================
            template <typename T, template <class> class POINT>
            class Pair : public Object
            {
            public:
                //==============================================================
                //
                // types and declarations
                //
                //==============================================================
                Y_EUCLIDEAN_ARGS();                      //!< alias
                typedef intr_ptr<PairKey,Pair> Pointer;  //!< alias

                //==============================================================
                //
                // members
                //
                //==============================================================
                const SharedPoint tail;
                const SharedPoint head;
                const_type        norm2;
                const_type        length;
                const PairKey     uuid;

                //==============================================================
                //
                // methods
                //
                //==============================================================
                //! cleanup
                inline virtual ~Pair() throw() {}

                //! setup from two shared points
                inline explicit Pair(const SharedPoint &a, const SharedPoint &b) throw() :
                tail(a), head(b), norm2(0), length(0), uuid( *tail, *head )
                {}

                //! update local data
                inline void update() throw()
                {
                    const VTX &A = (VTX &)(*tail);
                    const VTX &B = (VTX &)(*head);
                    const VTX  delta(A,B);
                    (mutable_type &)norm2  = delta.norm2();
                    (mutable_type &)length = math::sqrt_of(norm2);
                }

                //! key fot databases
                const PairKey & key() const throw() { return uuid; }

                //! using unique key to test equality
                friend inline bool  operator==( const Pair &lhs, const Pair &rhs ) throw()
                {
                    return lhs.uuid == rhs.uuid;
                }

                //! using uuid to test equality
                friend inline bool  operator!=( const Pair &lhs, const Pair &rhs ) throw()
                {
                    return lhs.uuid != rhs.uuid;
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Pair);
            };

            //==================================================================
            //
            //
            //! a segment is a dynamic, linked pair of points
            //
            //
            //==================================================================
            template <typename T, template <class> class POINT>
            class Segment : public Pair<T,POINT>::Pointer, public core::inode< Segment<T,POINT> >
            {
            public:
                //==============================================================
                //
                // types and declarations
                //
                //==============================================================
                Y_EUCLIDEAN_ARGS();                             //!< aliases
                typedef core::inode<Segment>       iNode;       //!< alias
                typedef Pair<T,POINT>              PairType;    //!< alias
                typedef typename PairType::Pointer SharedPair;  //!< alias
                typedef core::list_of_cpp<Segment> List;        //!< base class for Segments

                //==============================================================
                //
                // methods
                //
                //==============================================================
                //! cleanup
                inline virtual ~Segment() throw() {}
                //! create from an existing pair
                inline explicit Segment( const SharedPair &sharedPair ) throw() : SharedPair(sharedPair), iNode() {}
                //! create a new pair
                inline explicit Segment( const SharedPoint &a, const SharedPoint &b) : SharedPair( new PairType(a,b) ), iNode() {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Segment);
            };

            //==================================================================
            //
            //
            //! a list of dynamic pairs of dynamic points
            //
            //
            //==================================================================
            template <typename T, template <class> class POINT>
            class Segments : public Segment<T,POINT>::List
            {
            public:
                //==============================================================
                //
                // types and declarations
                //
                //==============================================================
                Y_EUCLIDEAN_ARGS();                                    //!< aliases
                typedef Segment<T,POINT>                  SegmentType; //!< alias
                typedef typename SegmentType::SharedPair  SharedPair;  //!< alias
                typedef typename SegmentType::List        SegmentList; //!< alias

                //==============================================================
                //
                // methods
                //
                //==============================================================
                inline explicit Segments() throw() : SegmentList() {}    //!< setup
                inline virtual ~Segments() throw() {}                    //!< cleanup
                inline Segments(const Segments &_) : SegmentList(_) {}   //!< copy

                //! append a new segment
                inline Segments & operator<<( const SharedPair &sharedPair)
                {
                    this->push_back( new SegmentType(sharedPair) );
                    return *this;
                }

                //! create and append a new segement
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
