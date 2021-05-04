
#include "y/mpi++/mpi.hpp"


namespace upsylon
{
    mpi::commTicks:: commTicks() throw() :
    last(0),
    full(0)
    {
    }
    
    mpi:: commTicks:: ~commTicks() throw()
    {
        reset();
    }
    
    void mpi::commTicks:: reset() throw()
    {
        aliasing::_(last) = aliasing::_(full) = 0;
    }
    
    void mpi:: commTicks:: operator()(const uint64_t delta) throw()
    {
        aliasing::_(full) += ( aliasing::_(last) =delta);
    }
    


    
}
