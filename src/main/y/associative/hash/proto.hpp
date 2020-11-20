

//! \file

#ifndef Y_HASH_PROTO_INCLUDED
#define Y_HASH_PROTO_INCLUDED 1

#include "y/associative/hash/table.hpp"


namespace upsylon
{

    //! prototype of hash_table based container

    template <
    typename KEY,
    typename T,
    typename NODE,
    typename KEY_HASHER,
    typename BASE_TYPE>
    class hash_proto : public BASE_TYPE
    {
    public:
        Y_DECL_ARGS(T,type);
        Y_DECL_ARGS(KEY,key_type);
        typedef NODE      node_type;
        typedef BASE_TYPE base_type;
        
        inline virtual ~hash_proto() throw() {}
        
        
        
        
    protected:
        inline explicit hash_proto() :
        base_type(),
        table(),
        hash()
        {}
        
        hash_table<NODE>  table;
        
    public:
        mutable KEY_HASHER hash;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(hash_proto);
    };

}

#endif
