
//! \file

#ifndef Y_HASH_SET_INCLUDED
#define Y_HASH_SET_INCLUDED 1

#include "y/associative/hash/proto.hpp"

namespace upsylon
{
    //__________________________________________________________________________
    //
    //
    //! node for hash_set
    //
    //__________________________________________________________________________

    template <typename KEY, typename T>
    class hash_set_node
    {
    public:
        Y_DECL_ARGS(T,type);       //!< aliases
        Y_DECL_ARGS(KEY,key_type); //!< aliases

        hash_set_node *next; //!< for list
        hash_set_node *prev; //!< for list
        hash_meta     *meta; //!< for table
        type           data; //!< actual data

        //! setup
        inline hash_set_node(const_key_type &, const_type &user_data) :
        next(0), prev(0), meta(0), data(user_data)
        {
        }

        //! copy
        inline hash_set_node(const hash_set_node &node) :
        next(0), prev(0), meta(0), data(node.data)
        {
        }

        //! cleanup
        inline ~hash_set_node() throw() {}

        //! madatory for table
        inline const_key_type & key() const throw()
        {
            return data.key();
        }

        type &       operator*()       throw() { return data; } //!< access
        const_type & operator*() const throw() { return data; } //!< acess

    private:
        Y_DISABLE_ASSIGN(hash_set_node);
    };

}

#include "y/associative/catalog.hpp"

namespace upsylon
{
    //__________________________________________________________________________
    //
    //
    //! hash_set
    //
    //__________________________________________________________________________
    template <typename KEY, typename T, typename KEY_HASHER = key_hasher<KEY> >
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

        //! cleanup
        inline virtual ~hash_set() throw() {}

        //! setup
        inline explicit hash_set(const size_t r=hash_table_::default_load) : prototype(r)
        {
        }

        //! setup with memory
        inline explicit hash_set(const size_t n, const as_capacity_t &_, const size_t r=hash_table_::default_load) :
        prototype(n,_,r)
        {
        }

        //! copy
        inline hash_set(const hash_set &other) :
        collection(),
        prototype(other)
        {
        }

        //! assign by copy/swap
        inline hash_set & operator=(const hash_set &other)
        {
            hash_set temp(other);
            this->swap_with(temp);
            return *this;
        }


        //______________________________________________________________________
        //
        //! catalog interface
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
