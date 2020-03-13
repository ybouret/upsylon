
//! \file

#ifndef Y_SUFFIX_STORE_INCLUDED
#define Y_SUFFIX_STORE_INCLUDED 1

#include "y/core/list.hpp"
#include "y/type/args.hpp"
#include "y/object.hpp"

namespace upsylon {

    template <typename T>
    class suffix_store
    {
    public:
        Y_DECL_ARGS(T,type);
        class node_type;
        typedef core::list_of_cpp<node_type> node_list;

        class node_type : public object
        {
        public:
            node_type  *next;
            node_type  *prev;
            node_list   chld;
            const_type  code;

            inline explicit node_type(const_type args) throw() :
            next(0), prev(0), code(args), chld()
            {
            }

            inline virtual ~node_type() throw()
            {
            }



        private:
            Y_DISABLE_COPY_AND_ASSIGN(node_type);
        };

        inline explicit suffix_store() : root( new node_type(0) )
        {
        }

        inline virtual ~suffix_store() throw()
        {
            delete root;
            root = 0;
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(suffix_store);
        node_type *root;
    };

}

#endif

