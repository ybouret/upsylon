//! \file
#ifndef Y_ASSOCIATIVE_MAP_INCLUDED
#define Y_ASSOCIATIVE_MAP_INCLUDED 1

#include "y/associative/hash-container.hpp"

namespace upsylon
{
    namespace core
    {
        //! internal map node
        template <
        typename KEY,
        typename T>
        class map_node
        {
        public:
            Y_DECL_ARGS(KEY,key_type); //!< alias
            Y_DECL_ARGS(T,type);       //!< alias

            map_node      *next; //!< for slots
            map_node      *prev; //!< for slots
            const_key_type key_; //!< key hard copy
            const size_t   hkey; //!< hash(key_)
            type           data; //!< effective data
            void          *meta; //!< meta node

            //! node API for hash_table
            inline const_key_type & key() const throw() { return key_; }

            //! destructor
            inline ~map_node() throw()
            {
                assert(NULL==next);
                assert(NULL==prev);
                meta = NULL;
                (size_t&)hkey = 0;
            }

            //! build
            inline  map_node(param_key_type k, const size_t h, param_type args) :
            next(0), prev(0), key_(k), hkey(h), data(args), meta(0) {}

            //! copy constructor
            inline map_node(const map_node &other) :
            next(0), prev(0), key_(other.key_), hkey(other.hkey), data(other.data), meta(0) {}


        private:
            Y_DISABLE_ASSIGN(map_node);
        };
    }

    //! relational map
    template <
    typename KEY,
    typename T,
    typename KEY_HASHER = key_hasher<KEY>,
    typename ALLOCATOR  = memory::global >
    class map : public hash_container<KEY,T, core::map_node<KEY,T>, KEY_HASHER, ALLOCATOR >
    {
    public:
        typedef core::map_node<KEY,T>                                node_type; //!< internal node
        typedef hash_container<KEY,T,node_type,KEY_HASHER,ALLOCATOR> base_type; //!< hash_table wrapper
        Y_DECL_ARGS(KEY,key_type); //!< alias
        Y_DECL_ARGS(T,type);       //!< alias

        //! default constructor
        inline explicit map() throw() : base_type() {}

        //! constructor with memoru
        inline explicit map(const size_t n, const as_capacity_t &_) : base_type(n,_) {}

        //! copy constructor
        inline map(const map &other) : collection(), base_type(other) {}

        //! assign from base type
        inline map & operator=( const map &other )
        {
            static_cast<base_type&>(*this) = other;
            return *this;
        }


        //! desctructor
        inline virtual ~map() throw() {}

        //! insert args with key, return false if key already in
        inline bool insert( param_key_type k, param_type args )
        {
            const size_t h = this->hash(k);
            return this->table.template insert<key_type,type>(k,h,args);
        }

        //! output key:T
        inline friend std::ostream & operator<<( std::ostream &os, const map &s )
        {
            os << '{';
            const size_t n = s.size();
            size_t       i = 1;
            for( typename base_type::const_iterator j=s.begin();i<=n;++i,++j)
            {
                os << j.key() << ':' << *j;
                if(i<n) os << ',';
            }
            os << '}';
            return os;
        }

    };


}

#endif
