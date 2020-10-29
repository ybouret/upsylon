//! \file
#ifndef Y_PREFIX_STEM_INCLUDED
#define Y_PREFIX_STEM_INCLUDED 1

#include "y/associative/prefix/node.hpp"

namespace upsylon {

    //__________________________________________________________________________
    //
    //
    //! a stem is a basic dynamic tree
    //
    //__________________________________________________________________________
    template <typename CODE, typename T>
    class prefix_stem
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        typedef          prefix_node<CODE,T>         node_type; //!< alias
        typedef typename prefix_node<CODE,T>::pool_t pool_type; //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        //! setup with root node
        inline explicit prefix_stem() :
        root( new node_type(0,0) ), pool()
        {
        }

        //! cleanup
        inline virtual ~prefix_stem() throw() { delete root; root=0; }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! get vizible root
        const ios::vizible & get_root() const throw() { assert(root); return *root; }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(prefix_stem);
        node_type *root;
        pool_type  pool;

    };

}

#endif

