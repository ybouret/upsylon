//! \file

#ifndef Y_FACTORY_INCLUDED
#define Y_FACTORY_INCLUDED 1


#include "y/associative/map.hpp"

namespace upsylon {

    namespace core {

        struct factory
        {
            static const char default_name[];
            static void throw_multiple_keys(const char *name);
            static void throw_no_creator(const char *name);
        };

    }

    template <
    typename T,
    typename KEY,
    typename CREATOR,
    typename KEY_HASHER = key_hasher<KEY>,
    typename ALLOCATOR  = memory::global>
    class factory
    {
    public:
        typedef  map<KEY,CREATOR,KEY_HASHER,ALLOCATOR> db_type;
        typedef  typename db_type::const_iterator      const_iterator;
        Y_DECL_ARGS(KEY,key_type); //!< alias


        virtual const char * name() const throw() { return core::factory::default_name; };

        inline explicit factory() throw() : db() {}
        inline virtual ~factory() throw() {}

        inline const_iterator begin() throw() { return static_cast<const db_type&>( db ).begin(); }
        inline const_iterator end()   throw() { return static_cast<const db_type&>( db ).end();   }

        inline void declare( param_key_type key, const CREATOR &creator )
        {
            if( !db.insert(key,creator) ) core::factory::throw_multiple_keys( name() );
        }

        const CREATOR *query(param_key_type key) const throw()
        {
            return db.search(key);
        }

        T * operator()( param_key_type key ) const
        {
            CREATOR &creator = find(key);
            return creator();
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

