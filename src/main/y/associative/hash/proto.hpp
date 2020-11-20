

//! \file

#ifndef Y_HASH_PROTO_INCLUDED
#define Y_HASH_PROTO_INCLUDED 1

#include "y/associative/hash/table


namespace upsylon
{

    //! prototype of hash_table based container

    template <typename KEY,typename T,typename NODE,typename BASE_CLASS>
    class hash_proto : public BASE_CLASS
    {
    public:

    private:
        Y_DISABLE_COPY_AND_ASSIGN(hash_proto);
    };

}

#endif
