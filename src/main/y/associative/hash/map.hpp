//! \file

#ifndef Y_HASH_MAP_INCLUDED
#define Y_HASH_MAP_INCLUDED 1

#include "y/associative/hash/proto.hpp"

namespace upsylon
{
    //__________________________________________________________________________
    //
    //
    //! node for hash_map
    //
    //__________________________________________________________________________
    template <typename KEY, typename T>
    class hash_map_node
    {
    public:
        Y_DECL_ARGS(T,type);       //!< aliases
        Y_DECL_ARGS(KEY,key_type); //!< aliases

        hash_map_node *next; //!< for list
        hash_map_node *prev; //!< for list
        hash_meta     *meta; //!< for table
        const_key_type ckey; //!< const key
        type           data; //!< actual data

        //! setup
        inline hash_map_node(const_key_type &user_key, const_type &user_data) :
        next(0), prev(0), meta(0), ckey(user_key), data(user_data)
        {
        }

        //! copy
        inline hash_map_node(const hash_map_node &node) :
        next(0), prev(0), meta(0), ckey(node.ckey), data(node.data)
        {
        }

        //! cleanup
        inline ~hash_map_node() throw() {}

        //! madatory for table
        inline const_key_type & key() const throw()
        {
            return ckey;
        }

        type &       operator*()       throw() { return data; } //!< access
        const_type & operator*() const throw() { return data; } //!< acess

    private:
        Y_DISABLE_ASSIGN(hash_map_node);
    };

}

#include "y/associative/lexicon.hpp"

namespace upsylon
{
    //__________________________________________________________________________
    //
    //
    //! hash_set
    //
    //__________________________________________________________________________
    template <typename KEY, typename T, typename KEY_HASHER = key_hasher<KEY> >
    class hash_map : public hash_proto<KEY,T, hash_map_node<KEY,T>, KEY_HASHER, lexicon<KEY,T> >
    {
    public:
        //______________________________________________________________________
        //
        // types and declaration
        //______________________________________________________________________
        Y_DECL_ARGS(T,type);                                                //!< aliases
        Y_DECL_ARGS(KEY,key_type);                                          //!< aliases
        typedef hash_map_node<KEY,T>                             node_type; //!< alias
        typedef lexicon<KEY,T>                                   base_type; //!< alias
        typedef hash_proto<KEY,T,node_type,KEY_HASHER,base_type> prototype; //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! cleanup
        inline virtual ~hash_map() throw() {}

        //! setup
        inline explicit hash_map() : prototype()
        {
        }

        //! setup with memory
        inline explicit hash_map(const size_t n, const as_capacity_t &_) :
        prototype(n,_)
        {
        }

        //! copy
        inline hash_map(const hash_map &other) :
        collection(),
        prototype(other)
        {
        }

        //! assign by copy/swap
        inline hash_map & operator=(const hash_map &other)
        {
            hash_map temp(other);
            this->swap_with(temp);
            return *this;
        }


        //______________________________________________________________________
        //
        //! catalog interface
        //______________________________________________________________________
        inline virtual bool insert(param_key_type key, param_type args)
        {
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
