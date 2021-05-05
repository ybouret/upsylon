
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
    mpi::commFlux_::  commFlux_() throw() : send(), recv() {}
    mpi::commFlux_:: ~commFlux_() throw() {}
    mpi::commFlux_::  commFlux_(const commFlux_ &_) throw() : send(_.send), recv(_.send) {}

    void mpi::commFlux_:: reset() throw()
    {
        send.reset();
        recv.reset();
    }

}

namespace upsylon
{
    mpi::commFlux:: ~commFlux() throw() {}

    mpi::commFlux:: commFlux(const commFlux &other) throw() :
    commFlux_(other), info(other.info)
    {
    }

    mpi::commFlux:: commFlux(const rtti &tid) throw() :
    commFlux_(),
    info(tid)
    {
    }
    


}
