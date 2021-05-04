
#include "y/mpi++/mpi.hpp"


namespace upsylon
{
    mpi::commTracer:: commTracer() throw() :
    last(0),
    full(0)
    {
    }
    
    mpi::commTracer:: commTracer(const commTracer &_) throw() :
    last(_.last),
    full(_.full)
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
    
    mpi::commTicks:: commTicks(const commTicks &_) throw() : commTracer(_) {}

}
namespace upsylon
{
    mpi::commBytes::  commBytes() throw() : commTracer() {}
    mpi::commBytes:: ~commBytes() throw() {}
    
    mpi::commBytes::  commBytes(const commBytes &_) throw() :
    commTracer(_)
    {}

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
    
    mpi:: commState:: commState(const commState &cs) throw() :
    ticks(cs.ticks), bytes(cs.bytes)
    {
    }
}
