//! \file
#ifndef Y_ORDERED_INCLUDED
#define Y_ORDERED_INCLUDED 1

#include "y/container/container.hpp"
#include "y/type/args.hpp"
namespace upsylon
{

    template <typename T>
    class ordered : public container
    {
    public:
        Y_DECL_ARGS(T,type); //!< aliases
        
        inline virtual ~ordered() throw() {}

    protected:
        inline explicit ordered() throw() {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(ordered);
    };

}


#endif
