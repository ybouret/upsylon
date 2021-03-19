
#include "y/gfx/edges/dvertex.hpp"

namespace upsylon
{
    namespace graphic
    {

        namespace edges
        {
            size_t dvertex:: index_of(const type &vtx) throw()
            {
                static const unsigned xp = 0x01;
                static const unsigned xm = 0x02;
                static const unsigned yp = 0x04;
                static const unsigned ym = 0x08;

                unsigned flags = 0x00;
                if(vtx.x>0) flags |= xp; else { if(vtx.x<0) flags |= xm; }
                if(vtx.y>0) flags |= yp; else { if(vtx.y<0) flags |= ym; }

                switch(flags)
                {
                    case xp: return 1;
                    case yp: return 2;
                    case xm: return 3;
                    case ym: return 4;

                    case xp|yp: return 5;
                    case xm|yp: return 6;
                    case xm|ym: return 7;
                    case xp|ym: return 8;

                    default:
                        break;
                }

                return 0;
            }
        }

    }

}

