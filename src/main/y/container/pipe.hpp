//! \file
#ifndef Y_CONTAINER_PIPE_INCLUDED
#define Y_CONTAINER_PIPE_INCLUDED 1

#include "y/container/container.hpp"
#include "y/type/args.hpp"

namespace upsylon
{
    //! base class for a pipe (FIFO/LIFO)
    template <typename T>
    class pipe : public container
    {
    public:
        Y_DECL_ARGS(T,type); //!< alias

        //! destructor
        inline virtual ~pipe() throw() {}

        //! store an object
        virtual void store(param_type) = 0;

        //! query and object and remove it
        virtual type query()

    protected:
        inline explicit pipe() throw() : container() {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(pipe);
    };
}

#endif

