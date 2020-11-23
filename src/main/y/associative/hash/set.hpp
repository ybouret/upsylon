
//! \file

#ifndef Y_HASH_SET_INCLUDED
#define Y_HASH_SET_INCLUDED 1

#include "y/associative/hash/proto.hpp"

namespace upsylon
{

    template <typename KEY, typename T>
    class hash_set_node
    {
    public:
        Y_DECL_ARGS(T,type);       //!< aliases
        Y_DECL_ARGS(KEY,key_type); //!< aliases

        hash_set_node *next;
        hash_set_node *prev;
        hash_meta     *meta;
        type           data;

        inline hash_set_node(const_key_type &, const_type &user_data) :
        next(0), prev(0), meta(0), data(user_data)
        {
        }

        inline hash_set_node(const hash_set_node &node) :
        next(0), prev(0), meta(0), data(node.data)
        {
        }

        inline ~hash_set_node() throw() {}

        inline const_key_type & key() const throw()
        {
            return data.key();
        }

    private:
        Y_DISABLE_ASSIGN(hash_set_node);
    };

}

#include "y/associative/catalog.hpp"

namespace upsylon
{
    template <typename KEY, typename T, typename KEY_HASHER = key_hasher<T> >
    class hash_set : public hash_proto<KEY,T, hash_set_node<KEY,T>, KEY_HASHER, catalog<KEY,T> >
    {
    public:
        //______________________________________________________________________
        //
        // types and declaration
        //______________________________________________________________________
        Y_DECL_ARGS(T,type);                                                //!< aliases
        Y_DECL_ARGS(KEY,key_type);                                          //!< aliases
        typedef hash_set_node<KEY,T>                             node_type; //!< alias
        typedef catalog<KEY,T>                                   base_type; //!< alias
        typedef hash_proto<KEY,T,node_type,KEY_HASHER,base_type> prototype; //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        inline virtual ~hash_set() throw() {}

        inline explicit hash_set() : prototype()
        {
        }

        inline explicit hash_set(const size_t n, const as_capacity_t &_) :
        prototype(n,_)
        {
        }

        inline hash_set(const hash_set &other) :
        collection(),
        prototype(other)
        {
        }

        inline hash_set & operator=(const hash_set &other)
        {
            hash_set temp(other);
            this->swap_with(temp);
            return *this;
        }


        //______________________________________________________________________
        //
        // catalog interface
        //______________________________________________________________________
        inline virtual bool insert(param_type args)
        {
            const_key_type &key  = args.key();
            const size_t    hkey = this->hash(key);
            if(this->table. template insert<KEY>(key,hkey,args))
            {
                this->post_insert();
                return true;
            }
            else
            {
                return false;
            }
        }


    };
}

#endif
