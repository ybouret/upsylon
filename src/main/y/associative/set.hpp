
//! \file
#ifndef Y_ASSOCIATIVE_SET_INCLUDED
#define Y_ASSOCIATIVE_SET_INCLUDED 1

#include "y/associative/hash-container.hpp"
#include "y/associative/catalog.hpp"
#include <iostream>

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
            inline ~set_node() throw()
            {
                assert(NULL==next);
                assert(NULL==prev);
                meta = NULL;
                (size_t&)hkey = 0;
            }

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
    class set : public hash_container<KEY,T,catalog<KEY,T>,core::set_node<KEY,T>, KEY_HASHER, ALLOCATOR >
    {
    public:
        Y_DECL_ARGS(KEY,key_type); //!< key alias
        Y_DECL_ARGS(T,type);       //!< alias

        typedef core::set_node<KEY,T>                                           node_type; //!< internal node
        typedef catalog<KEY,T>                                                  interface; //!< underlying interface
        typedef hash_container<KEY,T,interface,node_type,KEY_HASHER, ALLOCATOR> hash_type; //!< base type alias
        typedef typename hash_type::meta_node                                   meta_node; //!< meta node alias

        //! default constructor
        inline explicit set() throw() : hash_type() {}
        //! constructor with intitial memory
        inline explicit set(const size_t n, const as_capacity_t &_) : hash_type(n,_) {}
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

        //! copy constructor
        inline set( const set &other ) : collection(), hash_type(other) {}

        //! assign
        inline set & operator=( const set &other )
        {
            hash_type &self = *this;
            self = other;
            return *this;
        }

        //! output, user's defined T behavior
        inline friend std::ostream & operator<<( std::ostream &os, const set &s )
        {
            os << '{';
            const size_t n = s.size();
            size_t       i = 1;
            for( typename hash_type::const_iterator j=s.begin();i<=n;++i,++j)
            {
                os << *j;
                if(i<n) os << ',' << ' ';
            }
            os << '}';
            return os;
        }

    };

}

#endif
