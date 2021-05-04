
#include "y/mpi++/mpi.hpp"


namespace upsylon
{
    mpi::commTracer:: commTracer() throw() :
    last(0),
    full(0)
    {
    }


    mpi:: commTracer:: ~commTracer() throw()
    {
        reset();
    }
    
    void mpi::commTracer:: reset() throw()
    {
        aliasing::_(last) = aliasing::_(full) = 0;
    }
    
    void mpi:: commTracer:: operator()(const uint64_t delta) throw()
    {
        aliasing::_(full) += ( aliasing::_(last) =delta);
    }

    
}

namespace upsylon
{
    mpi::commTicks:: commTicks() throw() : commTracer() {}
    mpi::commTicks:: ~commTicks() throw() {}
}
namespace upsylon
{
    mpi::commBytes::  commBytes() throw() : commTracer() {}
    mpi::commBytes:: ~commBytes() throw() {}
}

namespace upsylon
{
    mpi:: commState:: commState() throw() :
    ticks(), bytes()
    {
    }

    mpi::commState:: ~commState() throw()
    {
    }

    void mpi:: commState:: reset() throw()
    {
        ticks.reset();
        bytes.reset();
    }
}
