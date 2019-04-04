#include "y/os/hw.hpp"

extern "C" size_t upsylon_hardware_nprocs();

namespace upsylon
{

    size_t hardware:: nprocs()
    {
        return upsylon_hardware_nprocs();
    }

}
