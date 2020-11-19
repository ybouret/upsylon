
//! \file

#ifndef Y_HASH_BUCKET_INCLUDED
#define Y_HASH_BUCKET_INCLUDED 1

#include "y/associative/hash/handle.hpp"
#include "y/core/list.hpp"

namespace upsylon
{


    typedef core::list_of<hash_handle> hash_handles; //!< alias

    //__________________________________________________________________________
    //
    //
    //! list of handles
    //
    //__________________________________________________________________________
    class hash_bucket : public hash_handles
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        explicit hash_bucket() throw(); //!< setup
        virtual ~hash_bucket() throw(); //!< cleanup

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        void         push();                               //!< push_back a zero handle
        void         pop() throw();                        //!< remove back

        hash_handle *query(const size_t hkey, void *node) throw(); //!< with size>0
        void         store(hash_handle *handle)   throw(); //!< a used handle

        void         release() throw();                    //!< release all handles
        void         reorder() throw();                    //!< reorder by increasing address

    private:
        Y_DISABLE_COPY_AND_ASSIGN(hash_bucket);
    };


}

#endif
