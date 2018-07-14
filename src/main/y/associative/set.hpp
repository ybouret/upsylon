
//! \file
#ifndef Y_ASSOCIATIVE_SET_INCLUDED
#define Y_ASSOCIATIVE_SET_INCLUDED 1

#include "y/associative/hash-container.hpp"

namespace upsylon
{
    namespace core
    {
        template <
        typename KEY,
        typename T>
        class set_node
        {
        public:
            Y_DECL_ARGS(KEY,key_type);
            Y_DECL_ARGS(T,type);

            set_node      *next;
            set_node      *prev;
            const size_t   hkey;
            type           data;
            inline const_key_type & key() const throw() { return data.key(); }

            inline ~set_node() throw() {}


        private:
            Y_DISABLE_ASSIGN(set_node);
        };
    }

    template <
    typename KEY,
    typename T,
    typename KEY_HASHER = key_hasher<T>,
    typename ALLOCATOR  = memory::global >
    class set : public hash_container<KEY,T, core::set_node<KEY,T> >
    {
    public:
        typedef core::set_node<KEY,T>           node_type;
        typedef hash_container<KEY,T,node_type> base_type;

        inline explicit set() throw() : base_type() {}
        inline virtual ~set() throw() {}
        
    private:

    };

}

#endif
