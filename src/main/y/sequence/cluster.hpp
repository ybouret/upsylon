//! \file

#ifndef Y_SEQUENCE_CLUSTER_INCLUDED
#define Y_SEQUENCE_CLUSTER_INCLUDED 1

#include "y/collection.hpp"
#include "y/type/args.hpp"

namespace upsylon {


    template <typename T>
    class cluster : public virtual collection
    {
    public:
        Y_DECL_ARGS(T,type);

        inline virtual ~cluster() throw() {}

        virtual type &       operator[](const size_t) throw()       = 0;
        virtual const_type & operator[](const size_t) const throw() = 0;
        
    protected:
        inline explicit cluster() throw() {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(cluster);
    };

}

#endif
