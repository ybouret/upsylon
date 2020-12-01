
//! \file
#ifndef Y_GEOMETRIC_ARC_INCLUDED
#define Y_GEOMETRIC_ARC_INCLUDED 1

#include "y/mkl/geometric/segment.hpp"
#include "y/associative/hash/set.hpp"

namespace upsylon
{

    namespace mkl
    {

        namespace Geometric
        {

            class Arc_ : public Object
            {
            public:
                virtual ~Arc_() throw();

            protected:
                explicit Arc_() throw();

                void   nodeInsertFailure(const std::type_info &tid) const;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Arc_);
            };

            //__________________________________________________________________
            //
            //! base class for an arc
            //__________________________________________________________________
            template <typename T, template <class> class VTX>
            class Arc : public Arc_
            {
            public:
                //______________________________________________________________
                //
                // types and defintions
                //______________________________________________________________
                Y_DECL_ARGS(T,type);                              //!< aliases
                typedef Point<T,VTX>                 PointType;   //!< alias
                typedef typename PointType::Pointer  SharedPoint; //!< alias
                typedef typename PointType::vertex   vertex;      //!< alias
                typedef Node<T,VTX>                  NodeType;    //!< alias
                typedef typename NodeType::Pointer   SharedNode;  //!< alias
                typedef hash_set<NodeKey,SharedNode> Nodes;       //!< alias
                typedef Segment<T,VTX>               SegmentType; //!< alias
                typedef typename SegmentType::List   Segments;    //!< alias

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                //! cleanup
                inline virtual ~Arc() throw() {}

                //! insert new point
                virtual void insert(const SharedPoint &point) = 0;


                const Nodes    nodes;
                const Segments segments;

            protected:
                //! setup empty
                explicit Arc() : nodes(), segments()
                {
                }
                

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Arc);
            };

        }

    }

}

#endif
