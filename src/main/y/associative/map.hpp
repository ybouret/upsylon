//! \file
#ifndef Y_ASSOCIATIVE_MAP_INCLUDED
#define Y_ASSOCIATIVE_MAP_INCLUDED 1

#include "y/associative/hash-container.hpp"

namespace upsylon
{
    namespace core
    {
        template <
        typename KEY,
        typename T>
        class map_node
        {
        public:
            Y_DECL_ARGS(KEY,key_type);
            Y_DECL_ARGS(T,type);

            map_node      *next;
            map_node      *prev;
            const_key_type key_;
            const size_t   hkey;
            type           data;
            void          *meta;

            inline const_key_type & key() const throw() { return key_; }

            inline ~map_node() throw() {}
            inline  map_node(param_key_type k, const size_t h, param_type args) :
            next(0), prev(0), key_(k), hkey(h), data(args), meta(0) {}

            inline map_node(const map_node &other) :
            next(0), prev(0), key_(other.key_), hkey(other.hkey), data(other.data), meta(0) {}


        private:
            Y_DISABLE_ASSIGN(map_node);
        };
    }

    template <
    typename KEY,
    typename T,
    typename KEY_HASHER = key_hasher<T>,
    typename ALLOCATOR  = memory::global >
    class map : public hash_container<KEY,T, core::map_node<KEY,T> >
    {
    public:
        typedef core::map_node<KEY,T>           node_type;
        typedef hash_container<KEY,T,node_type> base_type;
        Y_DECL_ARGS(KEY,key_type);
        Y_DECL_ARGS(T,type);

        //! default constructor
        inline explicit map() throw() : base_type() {}

        //! constructor with memoru
        inline explicit map(const size_t n, const as_capacity_t &_) : base_type(n,_) {}

        //! copy constructor
        inline map(const map &other) : base_type(other) {}

        //! assign from base type
        inline map & operator=( const map &other )
        {
            static_cast<base_type&>(*this) = other;
            return *this;
        }


        //! desctructor
        inline virtual ~map() throw() {}

        inline bool insert( param_key_type k, param_type args )
        {
            const size_t h = this->hash(k);
            return this->table.template insert<key_type,type>(k,h,args);
        }

    };


}

#endif
