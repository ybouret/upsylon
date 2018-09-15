//! \file

#ifndef Y_MPI_INCLUDED
#define Y_MPI_INCLUDED 1

#include "y/string.hpp"
#include "y/exception.hpp"
#include <mpi.h>

namespace upsylon
{

    class mpi : public singleton<mpi>
    {
    public:
        static mpi & init( int * argc, char ***argv, int requestedThreadLevel );

    private:
        explicit mpi();
        virtual ~mpi() throw();
        friend class singleton<mpi>;

    public:
        static const  at_exit::longevity life_time = memory::pooled::life_time - 2;
    };

}

#endif

