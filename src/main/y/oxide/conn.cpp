#include "y/oxide/conn.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Oxide
    {

        const char *Conn :: Level2Text( const Level l ) throw()
        {
            switch (l)
            {
                case Level1: return "L1";
                case Level2: return "L2";
                case Level3: return "L3";
            }
            return "?";
        }

        const char * Conn::  Way2Text( const Way w ) throw()
        {
            switch(w)
            {
                case Forward: return "Forward";
                case Reverse: return "Reverse";
            }
            return "?";
        }

        Coord1D Conn::Sign( const Way w ) throw()
        {
            switch(w)
            {
                case Forward: return  1;
                case Reverse: return -1;
            }
            return 0;
        }

        Conn::Way Conn:: Opposite( const Way way ) throw()
        {
            switch(way)
            {
                case Forward: return Reverse;
                case Reverse: return Forward;
            }
        }




        static const char fn[] = "Oxide::Connectivity::";

        Conn::Level Conn::MakeLink(Coord1D       *direction,
                                   Coord1D       *orientation,
                                   Way           *way,
                                   const unsigned dims)
        {
            assert(direction);
            assert(orientation);
            assert(dims>=1);
            assert(dims<=3);
            assert(way);
            assert(Forward==*way);

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
                        *way  = Reverse; //!< first coordinate is negative
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


