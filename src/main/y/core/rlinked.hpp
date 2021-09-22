
//! \file
#ifndef Y_CORE_REF_LINKED_INCLUDED
#define Y_CORE_REF_LINKED_INCLUDED 1

#include "y/core/rnode.hpp"
#include "y/core/list.hpp"

namespace upsylon {

    //__________________________________________________________________________
    //
    //
    //! base class for a list of references
    //
    //__________________________________________________________________________
    template <typename T>
    class ref_dlist : public core::list_of_cpp< ref_dnode<T> >
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        typedef ref_dnode<T>                 node_type; //!< alias
        typedef core::list_of_cpp<node_type> list_type; //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit ref_dlist() throw() : list_type() {}            //!< setup
        inline virtual ~ref_dlist() throw() {}                          //!< cleanup
        inline          ref_dlist(const ref_dlist &_) : list_type(_) {}  //!< C++ copy

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! wrapper for push_back
        inline void append( typename node_type::param_type args)
        {
            this->push_back( new node_type(args) );
        }

        //! wrapper for push_front
        inline void prepend( typename node_type::param_type args)
        {
            this->push_front( new node_type(args) );
        }


    private:
        Y_DISABLE_ASSIGN(ref_dlist);
    };

}

#endif
