
//! \file

#ifndef Y_HASH_BUCKET_INCLUDED
#define Y_HASH_BUCKET_INCLUDED 1

#include "y/associative/hash/handle.hpp"
#include "y/core/list.hpp"

namespace upsylon
{

    typedef core::list_of<hash_handle> hash_handles;

    class hash_bucket : public hash_handles
    {
    public:
        explicit hash_bucket() throw();
        virtual ~hash_bucket() throw();

        void     push();        //!< push_back a zero handle
        void     pop() throw(); //!< remove back

        hash_handle *query(const size_t hkey, void *node);
        void         store(hash_handle *handle) throw();


        void release() throw();
        void reorder() throw();

    private:
        Y_DISABLE_COPY_AND_ASSIGN(hash_bucket);
    };


}

#endif
