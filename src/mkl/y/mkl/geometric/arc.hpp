
//! \file
#ifndef Y_GEOMETRIC_ARC_INCLUDED
#define Y_GEOMETRIC_ARC_INCLUDED 1

#include "y/mkl/geometric/node.hpp"
#include "y/associative/hash/set.hpp"

namespace upsylon
{

    namespace mkl
    {

        namespace Geometric
        {

            //__________________________________________________________________
            //
            //! base class for an arc
            //__________________________________________________________________
            template <typename T, template <class> class VTX>
            class Arc :
            public Object,
            public hash_set<NodeKey, typename Node<T,VTX>::Pointer >
            {
            public:
                //______________________________________________________________
                //
                // types and defintions
                //______________________________________________________________
                Y_DECL_ARGS(T,type);                              //!< aliases
                typedef Point<T,VTX>                 PointType;   //!< alias
                typedef typename PointType::vertex   vertex;      //!< alias
                typedef typename PointType::Pointer  SharedPoint; //!< alias
                typedef Node<T,VTX>                  NodeType;    //!< alias
                typedef typename NodeType::Pointer   SharedNode;  //!< alias
                typedef hash_set<NodeKey,SharedNode> Nodes;       //!< alias

                explicit Arc() : Nodes()
                {
                }


                virtual ~Arc() throw()
                {
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Arc);
            };

        }

    }

}

#endif
