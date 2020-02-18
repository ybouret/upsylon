//! \file

#ifndef Y_FACTORY_INCLUDED
#define Y_FACTORY_INCLUDED 1


#include "y/associative/map.hpp"

namespace upsylon {

    namespace core {

        //! base class for admin
        struct factory
        {
            static const char default_name[]; //!< "factory"
            static void throw_multiple_keys(const char *name); //!< throw
            static void throw_no_creator(const char *name); //!< throw
        };

    }

    //! fully configurable factory
    template <
    typename T,
    typename KEY,
    typename CREATOR,
    typename KEY_HASHER = key_hasher<KEY>,
    typename ALLOCATOR  = memory::global>
    class factory
    {
    public:
        typedef  map<KEY,CREATOR,KEY_HASHER,ALLOCATOR> db_type;         //!< alias
        typedef  typename db_type::const_iterator      const_iterator;  //!< alias
        Y_DECL_ARGS(KEY,key_type);                                      //!< aliases

        //! reconfigurable name
        virtual const char * name() const throw() { return core::factory::default_name; };

        inline explicit factory() throw() : db() {}                    //!< setup
        inline explicit factory(const size_t n) : db(n,as_capacity) {} //!< setup
        inline virtual ~factory() throw() {}                           //!< cleanup

        //! wrapper
        inline const_iterator begin() throw() { return static_cast<const db_type&>( db ).begin(); }

        //! wrapper
        inline const_iterator end()   throw() { return static_cast<const db_type&>( db ).end();   }

        //! wrapper
        inline size_t size() const throw() { return db.size(); }

        //! declare a new createor
        inline void declare( param_key_type key, const CREATOR &creator )
        {
            if( !db.insert(key,creator) ) core::factory::throw_multiple_keys( name() );
        }

        //! query if possible to create
        const CREATOR *query(param_key_type key) const throw()
        {
            return db.search(key);
        }

        //! zero-arg creator
        inline T * operator()( param_key_type key ) const
        {
            CREATOR &creator = find(key);
            return creator();
        }

        //! one arg creator
        template <typename U>
        inline T * operator()( param_key_type key, U &u ) const
        {
            CREATOR &creator = find(key);
            return creator(u);
        }


    private:
        db_type db;
        inline CREATOR & find( const key_type &key ) const
        {
            const CREATOR *p = db.search(key);
            if(!p) core::factory::throw_no_creator( name() );
            return (CREATOR &) *p;
        }
        Y_DISABLE_COPY_AND_ASSIGN(factory);
    };

}

#endif

