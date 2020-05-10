
#include "y/mpi++/mpi.hpp"


namespace upsylon
{
    mpi::comm_tracer:: comm_tracer() throw() :
    last(0),
    full(0)
    {
    }
    
    mpi:: comm_tracer:: ~comm_tracer() throw()
    {
        reset();
    }
    
    void mpi::comm_tracer:: reset() throw()
    {
        last = full = 0;
    }
    
    void mpi:: comm_tracer:: operator()(const uint64_t delta) throw()
    {
        full += (last=delta);
    }
    
    mpi::comm_ticks::  comm_ticks() throw() : comm_tracer()
    {
    }
    
    mpi::comm_ticks:: ~comm_ticks() throw()
    {
    }
    
    mpi::comm_data::  comm_data() throw() :
    comm_tracer(),
    type(MPI_DATATYPE_NULL)
    {
    }
    
    mpi::comm_data:: ~comm_data() throw()
    {
        type = MPI_DATATYPE_NULL;
    }
    
    mpi::comm_info:: comm_info() throw() : ticks(), data()
    {
    }
    
    mpi::comm_info::~comm_info() throw()
    {
    }
    
    void mpi::comm_info:: reset_all() throw()
    {
        ticks.reset();
        data.reset();
    }
    
}
