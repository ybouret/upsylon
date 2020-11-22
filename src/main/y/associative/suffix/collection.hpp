//! \file

#ifndef Y_SUFFIX_COLLECTION_INCLUDED
#define Y_SUFFIX_COLLECTION_INCLUDED 1

#include "y/collection.hpp"

namespace upsylon
{

    //__________________________________________________________________________
    //
    //
    //! minimal base class for suffix_graph
    //
    //__________________________________________________________________________=
    class suffix_collection : public virtual collection
    {
    public:
        virtual ~suffix_collection() throw(); //!< cleanup

    protected:
        explicit suffix_collection() throw(); //!< setup

    private:
        Y_DISABLE_COPY_AND_ASSIGN(suffix_collection);
    };
}

#endif
