//! \file

#ifndef Y_GRAPHIC_LINKED_INCLUDED
#define Y_GRAPHIC_LINKED_INCLUDED 1

#include "y/graphic/pixmap.hpp"
#include "y/core/node.hpp"
#include "y/core/list.hpp"
#include "y/core/pool.hpp"

namespace upsylon {

    namespace Graphic {

        typedef core::cpp_node_of<const Point> PNode;   //!< Point Node
        typedef core::pool_of_cpp<PNode>       PPool;   //!< Point Pool
        typedef core::list_of_cpp<PNode>       PList;   //!< Point List
        typedef Pixmap<size_t>                 Indices; //!< pixmap of indices

        //! operations on linked structures
        struct Linked
        {
            //! transfert a set of coordinates
            template <typename T>
            static inline
            void Transfer( Pixmap<T> &target, const Pixmap<T> &source, const PList &L )
            {
                for(const PNode *node=L.head;node;node=node->next)
                {
                    const Point p = **node;
                    target[p] = source[p];
                }
            }
        };

    }

}

#endif

