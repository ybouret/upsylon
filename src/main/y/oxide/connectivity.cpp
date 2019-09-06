#include "y/oxide/connectivity.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Oxide
    {

        static const char fn[] = "Oxide::Connectivity::";

        Connectivity::Level Connectivity::MakeLink(Coord1D       *direction,
                                                   Coord1D       *orientation,
                                                   const unsigned dims)
        {
            assert(direction);
            assert(orientation);
            assert(dims>=1);
            assert(dims<=3);

            unsigned     lvl   = 0;
            bool         neg   = false;

            for(unsigned dim = 0;dim<dims;++dim)
            {
                Coord1D &d = direction[dim];
                if(d<0)
                {
                    if(lvl<=0)
                    {
                        neg   = true;
                    }
                    d=-1;
                    ++lvl;
                    orientation[dim] = neg ? 1 : -1;
                }
                else if(0<d)
                {
                    if(lvl<=0)
                    {
                        neg   = false;
                    }
                    d = 1;
                    ++lvl;
                    orientation[dim] = neg ? -1 : 1;
                }
                else
                {
                    assert(0==d);
                    orientation[dim] = 0;
                }
            }

            switch(lvl)
            {
                case 1: return Level1;
                case 2: return Level2;
                case 3: return Level3;
                default: break;
            }

            throw exception("%snMakeLink(null direction)",fn);

        }

    }

}


