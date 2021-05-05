
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
    mpi::commFlux::  commFlux() throw() : send(), recv() {}
    mpi::commFlux:: ~commFlux() throw() {}
    mpi::commFlux::  commFlux(const commFlux &_) throw() : send(_.send), recv(_.send) {}

    void mpi::commFlux:: reset() throw()
    {
        send.reset();
        recv.reset();
    }

}

