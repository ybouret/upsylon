
//! \file
#ifndef Y_ASSOCIATIVE_SET_INCLUDED
#define Y_ASSOCIATIVE_SET_INCLUDED 1

#include "y/associative/hash-container.hpp"

namespace upsylon
{
    namespace core
    {
        //! internal node for set
        template <
        typename KEY,
        typename T>
        class set_node
        {
        public:
            Y_DECL_ARGS(KEY,key_type); //!< alias
            Y_DECL_ARGS(T,type);       //!< alias

            set_node      *next; //!< for list
            set_node      *prev; //!< for list
            const size_t   hkey; //!< hash of data.key()
            type           data; //!< effective data
            void          *meta; //!< address of meta node
            //! for hash table code
            inline const_key_type & key() const throw() { return data.key(); }
            //! destructpr
            inline ~set_node() throw() {}
            //! constructor
            inline  set_node(param_key_type,const size_t h,param_type args) :
            next(0),prev(0),hkey(h),data(args),meta(0)
            {
            }

            //! copy constructor
            inline set_node(const set_node &other) :
            next(0),prev(0),hkey(other.hkey),data(other.data),meta(0)
            {
            }


        private:
            Y_DISABLE_ASSIGN(set_node);
        };
    }

    //! set of object with own key
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

        typedef core::set_node<KEY,T>           node_type; //!< internal node
        typedef hash_container<KEY,T,node_type> base_type; //!< base type alias

        //! default constructor
        inline explicit set() throw() : base_type() {}
        //! constructor with intitial memory
        inline explicit set(const size_t n, const as_capacity_t &_) : base_type(n,_) {}
        //! destructor
        inline virtual ~set() throw() {}
        //! data insertion
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
