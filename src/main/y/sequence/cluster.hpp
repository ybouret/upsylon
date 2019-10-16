//! \file

#ifndef Y_SEQUENCE_CLUSTER_INCLUDED
#define Y_SEQUENCE_CLUSTER_INCLUDED 1

#include "y/collection.hpp"
#include "y/type/args.hpp"

namespace upsylon {


    //! collection of addressable items
    template <typename T>
    class cluster : public virtual collection
    {
    public:
        Y_DECL_ARGS(T,type);                                             //!< aliases
        inline virtual ~cluster() throw() {}                             //!< cleanup
        virtual type &       operator[](const size_t) throw()       = 0; //!< access
        virtual const_type & operator[](const size_t) const throw() = 0; //!< const access
    protected:                                                           //|
        inline explicit cluster() throw() {}                             //!< setup
    private:
        Y_DISABLE_COPY_AND_ASSIGN(cluster);
    };

}

#endif
