//! \file

#ifndef Y_CONTAINER_COUNTED
#define Y_CONTAINER_COUNTED 1

#include "y/ptr/counted.hpp"

namespace upsylon {

    template <typename CONTAINER>
    class make_counted  : public counted_object, public CONTAINER
    {
    public:
        typedef typename CONTAINER::type         type;
        typedef typename CONTAINER::mutable_type mutable_type;
        typedef typename CONTAINER::const_type   const_type;

        inline virtual ~make_counted() throw() {}
        inline explicit make_counted() : CONTAINER() {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(make_counted);

    };

}


#endif

