
//! \file
#ifndef Y_GEOMETRIC_ARC_INCLUDED
#define Y_GEOMETRIC_ARC_INCLUDED 1

#include "y/mkl/geometric/segment.hpp"
#include "y/sequence/vector.hpp"
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
            class Arc : public Object
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
                typedef Node<T,VTX>                      NodeType;
                typedef typename NodeType::Pointer       SharedNode;
                typedef vector<SharedNode>               Nodes;
                typedef Segment<T,VTX>                   SegmentType;
                typedef vector<SegmentType>              Segments;
                static const size_t Dimensions = PointType::Dimensions;
                
                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                //! cleanup
                inline virtual ~Arc() throw() {}

                virtual void insert( const SharedPoint &point ) = 0;

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const Nodes    nodes;
                const Segments segments;

            protected:
                //! setup empty
                explicit Arc() : nodes(), segments()
                {
                }

                static inline
                void fill_bulk(matrix<type> &lhs,
                               const size_t col,
                               const type   factor,
                               const vertex &pm,
                               const vertex &p0,
                               const vertex &pp) throw()
                {
                    const vertex d = factor * ( (pm-p0) + (pp-p0) );
                    const type  *v = (const type*)&d;
                    for(size_t row=1,pos=0;pos<Dimensions;++pos,++row)
                    {
                        lhs[row][col] = v[pos];
                    }
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Arc);
            };

        }

    }

}

#endif
