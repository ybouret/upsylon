
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
            void          *meta;
            inline const_key_type & key() const throw() { return data.key(); }

            inline ~set_node() throw() {}
            inline  set_node(param_key_type,const size_t h,param_type args) :
            next(0),prev(0),hkey(h),data(args),meta(0)
            {
            }

            inline set_node(const set_node &other) :
            next(0),prev(0),hkey(other.hkey),data(other.data),meta(0)
            {
            }


        private:
            Y_DISABLE_ASSIGN(set_node);
        };
    }

    template <
    typename KEY,
    typename T,
    typename KEY_HASHER = key_hasher<KEY>,
    typename ALLOCATOR  = memory::global >
    class set : public hash_container<KEY,T, core::set_node<KEY,T> >
    {
    public:
        Y_DECL_ARGS(KEY,key_type); //!< key alias
        Y_DECL_ARGS(T,type);       //!< alias

        typedef core::set_node<KEY,T>           node_type;
        typedef hash_container<KEY,T,node_type> base_type;

        inline explicit set() throw() : base_type() {}
        inline explicit set(const size_t n, const as_capacity_t &_) : base_type(n,_) {}
        inline virtual ~set() throw() {}

        inline bool insert( param_type args )
        {
            const_type     &t = args;
            const_key_type &k = t.key();
            const size_t    h = this->hash(k);
            return this->table.template insert<key_type,type>(k,h,args);
        }

    };

}

#endif
