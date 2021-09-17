
//! \file
#ifndef Y_CORE_REF_LINKED_INCLUDED
#define Y_CORE_REF_LINKED_INCLUDED 1

#include "y/core/rnode.hpp"
#include "y/core/list.hpp"

namespace upsylon {

    template <typename T>
    class ref_list : public core::list_of_cpp< ref_dnode<T> >
    {
    public:
        typedef ref_dnode<T>                 node_type;
        typedef core::list_of_cpp<node_type> list_type;

        inline explicit ref_list() throw() : list_type() {}
        inline virtual ~ref_list() throw() {}
        inline          ref_list(const ref_list &other) : list_type(other) {}

        inline void append( typename node_type::param_type args)
        {
            this->push_back( new node_type(args) );
        }

        inline void prepend( typename node_type::param_type args)
        {
            this->push_front( new node_type(args) );
        }


    private:
        Y_DISABLE_ASSIGN(ref_list);
    };

}

#endif
