#include "y/mkl/solve/zircon.hpp"

namespace upsylon
{
    namespace mkl
    {

        const char zircon::label[] = "[zircon] ";
        zircon:: ~zircon() throw() {}
        zircon::  zircon() throw() {}

#define Y_ZIRCON_STATUS(NAME) case NAME: return #NAME
        const char * zircon:: status_name(const status s) throw()
        {
            switch (s)
            {
                    Y_ZIRCON_STATUS(success);
                    Y_ZIRCON_STATUS(stalled);
                    Y_ZIRCON_STATUS(singular);
                    Y_ZIRCON_STATUS(spurious);
            }
            return "";
        }

    }
}
