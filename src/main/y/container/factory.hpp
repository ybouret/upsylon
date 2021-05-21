//! \file

#ifndef Y_FACTORY_INCLUDED
#define Y_FACTORY_INCLUDED 1


#include "y/associative/hash/map.hpp"
#include "y/string.hpp"

namespace upsylon {



    //__________________________________________________________________________
    //
    //
    //! base class for factory<...>
    //
    //__________________________________________________________________________
    class factory_
    {
    public:
        static const at_exit::longevity minimum_life_time;  //!< from rtti
        virtual                        ~factory_() throw(); //!< cleanup
        const string                    typeName;           //!< from rtti


    protected:
        explicit factory_(const std::type_info &); //!< initialize
        void     throw_multiple_keys() const;      //!< throw
        void     throw_no_creator()    const;      //!< throw
    };


    //__________________________________________________________________________
    //
    //
    //! fully configurable factory
    //
    //__________________________________________________________________________
    template <
    typename T,
    typename KEY,
    typename CREATOR    = T * (*)(),
    typename KEY_HASHER = key_hasher<KEY>
    >
    class factory : public factory_
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        typedef  hash_map<KEY,CREATOR,KEY_HASHER> db_type;         //!< alias
        typedef  typename db_type::const_iterator const_iterator;  //!< alias
        Y_DECL_ARGS(KEY,key_type);                                 //!< aliases
        Y_DECL_ARGS(T,type);                                       //!< aliases

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! default setup
        inline explicit factory() : factory_(typeid(type)), db() {}

        //! setup with capacity
        inline explicit factory(const size_t n) : factory_(typeid(type)),db(n,as_capacity) {}

        //! cleanup
        inline virtual ~factory() throw() {}
        

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        //! wrapper
        inline const_iterator begin() throw() { return static_cast<const db_type&>( db ).begin(); }

        //! wrapper
        inline const_iterator end()   throw() { return static_cast<const db_type&>( db ).end();   }

        //! wrapper
        inline size_t         size() const throw() { return db.size(); }

        //! declare a new creator
        inline void declare(param_key_type key, const CREATOR &creator)
        {
            if( !db.insert(key,creator) ) throw_multiple_keys();
        }

        //! query if possible to create
        const CREATOR *look_for(param_key_type key) const throw()
        {
            return db.search(key);
        }

        //! zero-arg creator
        inline T * create(param_key_type key) const
        {
            CREATOR &creator = find(key);
            return creator();
        }

        //! one arg creator
        template <typename U>
        inline T * create( param_key_type key, U &u ) const
        {
            CREATOR &creator = find(key);
            return creator(u);
        }


    private:
        db_type db;
        inline CREATOR & find( const key_type &key ) const
        {
            const CREATOR *p = db.search(key);
            if(!p)  throw_no_creator();
            return (CREATOR &) *p;
        }
        Y_DISABLE_COPY_AND_ASSIGN(factory);
    };

}

#endif

