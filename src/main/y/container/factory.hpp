//! \file

#ifndef Y_FACTORY_INCLUDED
#define Y_FACTORY_INCLUDED 1


#include "y/associative/hash/map.hpp"
#include "y/container/mill.hpp"


namespace upsylon
{
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
    class factory : public mill, public hash_map<KEY,CREATOR,KEY_HASHER>
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
        inline explicit factory() : mill(typeid(type)), db_type() {}

        //! setup with capacity
        inline explicit factory(const size_t n) : mill(typeid(type)), db_type(n,as_capacity) {}

        //! cleanup
        inline virtual ~factory() throw() {}

        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(factory);
    };

}

#endif

