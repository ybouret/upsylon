//! \file
#ifndef Y_RELEASABLE_INCLUDED
#define Y_RELEASABLE_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{

    //! releasable object prototype
    class releasable
    {
    public:
        inline virtual ~releasable() throw() {} //!< destructor
        virtual void release() throw() = 0;     //!< release method

    protected:
        inline releasable() throw() {} //!< constructor

    private:
        Y_DISABLE_COPY_AND_ASSIGN(releasable);
    };
};

#endif

